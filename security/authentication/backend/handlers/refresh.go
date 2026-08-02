package handlers

import (
	"net/http"

	"auth-methods/backend/token"
)

func RefreshHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}

	// read refresh token from HttpOnly cookie
	cookie, err := r.Cookie("refresh_token")
	if err != nil {
		http.Error(w, "refresh_token cookie missing", http.StatusUnauthorized)
		return
	}

	accessTok, refreshTok, err := token.RotateRefreshToken(cookie.Value)
	if err != nil {
		http.Error(w, "unauthorized: "+err.Error(), http.StatusUnauthorized)
		return
	}

	setTokenCookies(w, accessTok, refreshTok)
	w.WriteHeader(http.StatusNoContent)
}
