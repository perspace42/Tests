'''
Author: Scott Field
Date: 08/31/25
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

# Set Brave browser binary location
options = Options()
options.binary_location = "/usr/bin/brave-browser"

# Create driver without explicitly specifying driver path or service
driver = webdriver.Chrome(options=options)

# Use the driver as usual
driver.get("https://www.selenium.dev/selenium/web/web-form.html")
driver.implicitly_wait(0.5)  # implicit wait

text_box = driver.find_element(By.NAME, "my-text")
submit_button = driver.find_element(By.CSS_SELECTOR, "button")

text_box.send_keys("Selenium")
submit_button.click()

message = driver.find_element(By.ID, "message")
print(message.text)

driver.quit()
