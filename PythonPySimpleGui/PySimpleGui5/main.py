'''
Author: Scott Field
Date: 09/01/25
Purpose:
Test edited PySimpleGUI5 example code
Dependencies:
- Python3.10
- PySimpleGUI
'''
import PySimpleGUI as sg

# All the stuff inside your window.
layout = [  [sg.Text("What's your name?")],
            [sg.InputText()],
            [sg.Button('Ok'), sg.Button('Cancel')] ]

# Create the Window
window = sg.Window('Hello Example', layout)

# Event Loop to process "events" and get the "values" of the inputs
while True:
    event, values = window.read()

    # if user closes window or clicks cancel
    if event == sg.WIN_CLOSED or event == 'Cancel':
        break

    print(f"Hello {values[0]} welcome to PySimpleGUI!")

window.close()
