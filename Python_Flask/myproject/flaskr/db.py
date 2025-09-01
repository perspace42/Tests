'''
Author: Scott Field
Date: 09/03/2024
Purpose:
Contains the application factory, and (since it is named __init__.py) causes the flaskr directory to be a python package
'''

import sqlite3

import click
from flask import current_app, g #g is an object used to store data that might be accessed by multiple functions during the request

#Get Database
def get_db():
    if 'db' not in g:
        g.db = sqlite3.connect(
            current_app.config['DATABASE'],
            detect_types=sqlite3.PARSE_DECLTYPES
        )
        g.db.row_factory = sqlite3.Row

    return g.db

#Close Database
def close_db(e=None):
    db = g.pop('db', None)

    if db is not None:
        db.close()

#Initialize Database
def init_db():
    db = get_db()

    #Open a file relative to the flaskr package (useful since it is not known where the flaskr package will be deployed)
    with current_app.open_resource('schema.sql') as f:
        db.executescript(f.read().decode('utf8'))

#Sets a Command Line Instruction To Initialize Database
@click.command('init-db')
def init_db_command():
    """Clear the existing data and create new tables."""
    init_db()
    click.echo('Initialized the database.')

#Register Commands With Application
def init_app(app):
    app.teardown_appcontext(close_db)
    app.cli.add_command(init_db_command)