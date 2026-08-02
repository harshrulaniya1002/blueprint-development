package main

import (
	"crypto/tls"
	"fmt"
	"net/http"
)

func main1() {

	router := http.NewServeMux()

	//routes
	router.HandleFunc("/test-connection", func(w http.ResponseWriter, r *http.Request) {
		// handler logic here
		fmt.Println("HTTPS request received")
		w.Write([]byte("HTTPS connection successful"))
	})

	tlsConfig := &tls.Config{
		
		// You can specify the minimum and maximum TLS versions if needed
		// MinVersion: tls.VersionTLS12,
        // MaxVersion: tls.VersionTLS12,
		CipherSuites: []uint16{
			// for TLS 1.2, you can specify the cipher suites you want to support
			// For example, you can use the following cipher suites:
			tls.TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
		},
	}

	server := &http.Server{
		Addr:      ":9999",
		Handler:   router,
		TLSConfig: tlsConfig, // You can configure TLS settings here if needed
	}

	if err := server.ListenAndServeTLS("./certs/rsa_certs/cert.pem", "./certs/rsa_certs/key.pem"); err != nil {
		fmt.Printf("Failed to start server: %v\n", err)
	}
}
