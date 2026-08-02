package main

import (
	"fmt"
	"net/http"
)

func main2() {

	router := http.NewServeMux()

    //routes
	router.HandleFunc("/fibs", func(w http.ResponseWriter, r *http.Request) {
		// handler logic here
		fmt.Printf("This Function is to check fibs complaint nature of your server\n")
		w.Write([]byte("This Function is to check fibs complaint nature of your server"))
	})
	
	http.ListenAndServe(":9999", router)
}
