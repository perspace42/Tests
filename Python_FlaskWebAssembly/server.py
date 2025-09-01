from flask import Flask, render_template, redirect
import threading
import time

app = Flask(__name__)

# Global variable to store the server thread
server_thread = None

def run_http_server():
    from http.server import HTTPServer, SimpleHTTPRequestHandler

    class CORSRequestHandler(SimpleHTTPRequestHandler):
        def end_headers(self):
            self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
            self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
            super().end_headers()  # Call the parent class's end_headers method

    httpd = HTTPServer(('localhost', 8000), CORSRequestHandler)
    print("Serving application: http://localhost:8000/QtWebAssemblyTest.html")
    httpd.serve_forever()

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/run-script', methods=['POST'])
def run_script():
    global server_thread
    if server_thread is None or not server_thread.is_alive():
        # Start the HTTP server in a new thread
        server_thread = threading.Thread(target=run_http_server)
        server_thread.start()
        
        # Wait a moment to ensure the server starts
        time.sleep(2)

    # Redirect to the WebAssembly application URL
    return redirect("http://localhost:8000/app/QtWebAssemblyTest.html")

if __name__ == '__main__':
    app.run(debug=True)