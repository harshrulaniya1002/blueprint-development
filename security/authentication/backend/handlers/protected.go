package handlers

import (
	"encoding/json"
	"net/http"
	"time"

	"auth-methods/backend/middleware"
	"auth-methods/backend/token"
)

type profileResponse struct {
	Username      string `json:"username"`
	Message       string `json:"message"`
	TokenExpiring bool   `json:"token_expiring_soon"`
}

// ProfileHandler is a protected route — middleware.RequireAuth must wrap it.
func ProfileHandler(w http.ResponseWriter, r *http.Request) {
	username, _ := r.Context().Value(middleware.UsernameKey).(string)

	// re-read cookie to check expiry proximity (already validated by middleware)
	expiringSoon := false
	if cookie, err := r.Cookie("access_token"); err == nil {
		if claims, err := token.ValidateToken(cookie.Value); err == nil {
			expiringSoon = token.IsExpiringSoon(claims, 3*time.Minute)
		}
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(profileResponse{
		Username:      username,
		Message:       "welcome to the protected route",
		TokenExpiring: expiringSoon,
	})
}
