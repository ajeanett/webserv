#!/usr/local/bin/python3

import random
import datetime

headers = {
	"Content-Type": "text/plain"
}

data = ""
try:
	data = input()
except EOFError:
	pass

html = f"""
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Test Page</title>
	<link rel="stylesheet" href="/styles.css">
</head>

<body>

<h1>Python Page!</h1>

<section>
	<div class="section-title">
		Random number
	</div>
	<div class="section-content">
		{random.randint(0, 1000)}
	</div>
</section>

<section>
	<div class="section-title">
		Current time
	</div>
	<div class="section-content">
		{datetime.datetime.now()}
	</div>
</section>

<section>
	<div class="section-title">
		STDIN Data
	</div>
	<div class="section-content">
		"{data}"
	</div>
</section>

<a class="go-home" href="/index.html">
	← Home
</a>

</body>
</html>
"""

for header in headers:
	print(header + ": " + headers[header], end="\r\n")
print("\r\n")
print(html)
