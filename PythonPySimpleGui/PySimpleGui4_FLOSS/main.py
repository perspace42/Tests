'''
Author: Scott Field
Date: 09/01/25
Purpose:
Test PySimpleGui4_FLOSS example code
Dependencies:
- Python3.10
- PySimpleGUI_4_FLOSS
'''
import PySimpleGUI as ui

# All the stuff inside your window.
layout = [  [ui.Text("What's your name?")],
            [ui.InputText()],
            [ui.Button('Ok'), ui.Button('Cancel')] ]

# Create the Window
window = ui.Window('Hello Example', layout)

# Event Loop to process "events" and get the "values" of the inputs
while True:
    event, values = window.read()

    # if user closes window or clicks cancel
    if event == ui.WIN_CLOSED or event == 'Cancel':
        break

    print(f"Hello {values[0]} welcome to PySimpleGUI_4_FLOSS!")

window.close()
