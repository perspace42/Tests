'''
Author: Scott Field
Date: 08/31/25
Purpose:
Test compatibility of Selenium with Brave Browser using the Chromium Driver
'''
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options

brave_path = "/usr/bin/brave-browser"
chromedriver_path = "/usr/bin/chrome-linux64"

options = Options()
options.binary_location = brave_path

service = Service(executable_path=chromedriver_path)
driver = webdriver.Chrome(service=service, options=options)

driver.get("https://www.selenium.dev/selenium/web/web-form.html")

driver.implicitly_wait(0.5)

text_box = driver.find_element(By.NAME, "my-text")
submit_button = driver.find_element(By.CSS_SELECTOR, "button")

text_box.send_keys("Selenium")
submit_button.click()

message = driver.find_element(By.ID, "message")
print(message.text)

driver.quit()
