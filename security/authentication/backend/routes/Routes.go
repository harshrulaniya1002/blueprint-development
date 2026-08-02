package routes

import (
	"net/http"

	"auth-methods/backend/handlers"
	"auth-methods/backend/middleware"
)

func SetupRoutes(router *http.ServeMux) {

	router.HandleFunc("/server-health", func(w http.ResponseWriter, r *http.Request) {
		w.Write([]byte("Server is healthy"))
	})

	// serve HTML pages
	router.HandleFunc("/login", func(w http.ResponseWriter, r *http.Request) {
		if r.Method == http.MethodGet {
			http.ServeFile(w, r, "./frontend/login.html")
			return
		}
		handlers.LoginHandler(w, r)
	})

	router.HandleFunc("/profile", func(w http.ResponseWriter, r *http.Request) {
		if r.Method == http.MethodGet {
			http.ServeFile(w, r, "./frontend/profile.html")
			return
		}
		middleware.RequireAuth(http.HandlerFunc(handlers.ProfileHandler)).ServeHTTP(w, r)
	})

	router.HandleFunc("/refresh", handlers.RefreshHandler)

	// JSON-only protected API endpoint
	router.Handle("/api/profile", middleware.RequireAuth(http.HandlerFunc(handlers.ProfileHandler)))
}
