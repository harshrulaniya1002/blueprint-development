package token

import (
	"errors"
	"os"
	"strconv"
	"sync"
	"time"

	"github.com/golang-jwt/jwt/v5"
)

func envDuration(key string, defaultSec int) time.Duration {
	if v := os.Getenv(key); v != "" {
		if sec, err := strconv.Atoi(v); err == nil {
			return time.Duration(sec) * time.Second
		}
	}
	return time.Duration(defaultSec) * time.Second
}

var (
	jwtSecret = func() string {
		if s := os.Getenv("JWT_SECRET"); s != "" {
			return s
		}
		return "super-secret-change-me"
	}()
	AccessTokenTTL  = envDuration("ACCESS_TOKEN_TTL_SECONDS", 15*60)
	RefreshTokenTTL = envDuration("REFRESH_TOKEN_TTL_SECONDS", 7*24*3600)
)

type Claims struct {
	Username string `json:"username"`
	jwt.RegisteredClaims
}

// refreshStore maps username → refresh token (in-memory only).
var (
	mu           sync.RWMutex
	refreshStore = make(map[string]string)
)

func GenerateAccessToken(username string) (string, error) {
	claims := Claims{
		Username: username,
		RegisteredClaims: jwt.RegisteredClaims{
			ExpiresAt: jwt.NewNumericDate(time.Now().Add(AccessTokenTTL)),
			IssuedAt:  jwt.NewNumericDate(time.Now()),
		},
	}
	return jwt.NewWithClaims(jwt.SigningMethodHS256, claims).SignedString([]byte(jwtSecret))
}

func GenerateRefreshToken(username string) (string, error) {
	claims := Claims{
		Username: username,
		RegisteredClaims: jwt.RegisteredClaims{
			ExpiresAt: jwt.NewNumericDate(time.Now().Add(RefreshTokenTTL)),
			IssuedAt:  jwt.NewNumericDate(time.Now()),
		},
	}
	tok, err := jwt.NewWithClaims(jwt.SigningMethodHS256, claims).SignedString([]byte(jwtSecret))
	if err != nil {
		return "", err
	}

	mu.Lock()
	refreshStore[username] = tok
	mu.Unlock()
	return tok, nil
}

func ValidateToken(tokenStr string) (*Claims, error) {
	tok, err := jwt.ParseWithClaims(tokenStr, &Claims{}, func(t *jwt.Token) (any, error) {
		if _, ok := t.Method.(*jwt.SigningMethodHMAC); !ok {
			return nil, errors.New("unexpected signing method")
		}
		return []byte(jwtSecret), nil
	})
	if err != nil {
		return nil, err
	}
	claims, ok := tok.Claims.(*Claims)
	if !ok || !tok.Valid {
		return nil, errors.New("invalid token")
	}
	return claims, nil
}

// RotateRefreshToken validates the incoming refresh token, issues a new pair,
// and invalidates the old one.
func RotateRefreshToken(incoming string) (accessTok, refreshTok string, err error) {
	claims, err := ValidateToken(incoming)
	if err != nil {
		return "", "", errors.New("invalid refresh token")
	}

	mu.RLock()
	stored := refreshStore[claims.Username]
	mu.RUnlock()

	if stored != incoming {
		return "", "", errors.New("refresh token reuse detected")
	}

	accessTok, err = GenerateAccessToken(claims.Username)
	if err != nil {
		return
	}
	refreshTok, err = GenerateRefreshToken(claims.Username) // overwrites old token in store
	return
}

// IsExpiringSoon returns true when the token expires within the given threshold.
func IsExpiringSoon(claims *Claims, threshold time.Duration) bool {
	return time.Until(claims.ExpiresAt.Time) < threshold
}
