package main

import (
	"auth-methods/backend/routes"
	"crypto/tls"
	"fmt"
	"log"
	"net/http"
	"time"

	"github.com/joho/godotenv"
)

func main() {
	// load .env — ignore error if file is absent (e.g. production uses real env vars)
	_ = godotenv.Load()

	router := http.NewServeMux()

	//setup Routes
	routes.SetupRoutes(router)

	cert, err := tls.LoadX509KeyPair("./backend/certs/cert.pem", "./backend/certs/key.pem")
	if err != nil {
		fmt.Printf("Failed to load server certificate: %v\n", err)
		return
	}

	tlsConfig := &tls.Config{
		MinVersion: tls.VersionTLS12,
		MaxVersion: tls.VersionTLS13,

		CipherSuites: []uint16{
			// TLS 1.2 FIPS-approved cipher suites
			tls.TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
			tls.TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384,
		},

		// You can also specify the server's certificate and private key if needed
		Certificates: []tls.Certificate{
			// Load your server certificate and private key here
			// For example:
			cert,
		},
	}

	server := &http.Server{
		Addr:      ":7777",
		Handler:   loggingMiddleware(router),
		TLSConfig: tlsConfig,
	}

	if err := server.ListenAndServeTLS("", ""); err != nil {
		fmt.Printf("Failed to start server: %v\n", err)
	}
}

type statusRecorder struct {
	http.ResponseWriter
	status int
}

func (sr *statusRecorder) WriteHeader(code int) {
	sr.status = code
	sr.ResponseWriter.WriteHeader(code)
}

func loggingMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		rec := &statusRecorder{ResponseWriter: w, status: http.StatusOK}
		start := time.Now()
		next.ServeHTTP(rec, r)
		log.Printf("%s %s %d %s", r.Method, r.URL.Path, rec.status, time.Since(start))
	})
}
