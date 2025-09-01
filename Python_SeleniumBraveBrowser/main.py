'''
Author: Scott Field
Date: 09/01/25
Purpose:
Test compatibility of Selenium with Brave Browser using the Chromium Driver
Dependencies:
- Python3.10
- Selenium
- webdriver_manager
- brave-browser
'''
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.by import By

#Get Brave Browser Location
options = Options()
options.binary_location = "/usr/bin/brave-browser"

#Use Chrome Driver For Brave Browser
driver = webdriver.Chrome(options=options)

#Use Driver To Get Form
driver.get("https://www.selenium.dev/selenium/web/web-form.html")
driver.implicitly_wait(0.5)

#Use Driver To Get Form Elements
text_box = driver.find_element(By.NAME, "my-text")
submit_button = driver.find_element(By.CSS_SELECTOR, "button")

#Send Data To Form Elements & Submit
text_box.send_keys("Selenium")
submit_button.click()

#Get Message Back From Form
message = driver.find_element(By.ID, "message")

#Print Message To Terminal
print(message.text)

#Ends WebDriver Session (Avoid Memory Leak)
driver.quit()
