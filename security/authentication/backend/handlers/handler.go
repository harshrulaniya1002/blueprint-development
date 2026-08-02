package handlers

import (
	"encoding/json"
	"net/http"

	"auth-methods/backend/token"
)

type LoginStruct struct {
	Username string `json:"username"`
	Password string `json:"password"`
}

type TokenResponse struct {
	AccessToken  string `json:"access_token"`
	RefreshToken string `json:"refresh_token"`
	ExpiresIn    int    `json:"expires_in_seconds"`
}

// setTokenCookies writes both tokens as HttpOnly Secure cookies.
func setTokenCookies(w http.ResponseWriter, accessTok, refreshTok string) {
	http.SetCookie(w, &http.Cookie{
		Name:     "access_token",
		Value:    accessTok,
		Path:     "/",
		MaxAge:   int(token.AccessTokenTTL.Seconds()),
		HttpOnly: true,
		Secure:   true,
		SameSite: http.SameSiteStrictMode,
	})
	http.SetCookie(w, &http.Cookie{
		Name:     "refresh_token",
		Value:    refreshTok,
		Path:     "/refresh",
		MaxAge:   int(token.RefreshTokenTTL.Seconds()),
		HttpOnly: true,
		Secure:   true,
		SameSite: http.SameSiteStrictMode,
	})
}

func LoginHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}

	var creds LoginStruct
	if err := json.NewDecoder(r.Body).Decode(&creds); err != nil {
		http.Error(w, "invalid request body", http.StatusBadRequest)
		return
	}

	// TODO: validate creds against a real user store
	if creds.Username == "" || creds.Password == "" {
		http.Error(w, "username and password required", http.StatusBadRequest)
		return
	}

	accessTok, err := token.GenerateAccessToken(creds.Username)
	if err != nil {
		http.Error(w, "could not generate access token", http.StatusInternalServerError)
		return
	}
	refreshTok, err := token.GenerateRefreshToken(creds.Username)
	if err != nil {
		http.Error(w, "could not generate refresh token", http.StatusInternalServerError)
		return
	}

	setTokenCookies(w, accessTok, refreshTok)
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(map[string]string{"redirect": "/profile"})
}
