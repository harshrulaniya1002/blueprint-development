package middleware

import (
	"context"
	"net/http"
	"strings"

	"auth-methods/backend/token"
)

type contextKey string

const UsernameKey contextKey = "username"

// RequireAuth reads the access_token cookie (falls back to Bearer header for API clients).
func RequireAuth(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		var rawToken string

		if cookie, err := r.Cookie("access_token"); err == nil {
			rawToken = cookie.Value
		} else if h := r.Header.Get("Authorization"); strings.HasPrefix(h, "Bearer ") {
			rawToken = strings.TrimPrefix(h, "Bearer ")
		} else {
			http.Error(w, "unauthorized", http.StatusUnauthorized)
			return
		}

		claims, err := token.ValidateToken(rawToken)
		if err != nil {
			http.Error(w, "unauthorized: "+err.Error(), http.StatusUnauthorized)
			return
		}

		ctx := context.WithValue(r.Context(), UsernameKey, claims.Username)
		next.ServeHTTP(w, r.WithContext(ctx))
	})
}
