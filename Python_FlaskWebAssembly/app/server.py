from http.server import HTTPServer, SimpleHTTPRequestHandler

class CORSRequestHandler(SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        super().end_headers()  # Call the parent class's end_headers method

httpd = HTTPServer(('localhost', 8000), CORSRequestHandler)
print("Serving application: http://localhost:8000/QtWebAssemblyTest.html")
httpd.serve_forever()