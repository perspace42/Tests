🟢 1. Best match: qgrid (Jupyter-based editable grid)
👉 qgrid
What it does
Displays a pandas DataFrame as a spreadsheet UI
Supports:
Sorting, filtering
Cell editing (double-click)
Works inside Jupyter notebooks

“Allows you to … edit your DataFrames by double clicking cells”

How it fits your use case
df = pd.read_sql("""
SELECT *
FROM orders o
JOIN customers c ON ...
""", conn)

qgrid_widget = qgrid.show_grid(df, show_toolbar=True)

👉 You get:

A single unified view across joins
Editable cells
⚠️ Limitation (critical)
Edits only modify the DataFrame in memory
You must manually push changes back to SQL
🟢 2. Shiny for Python (interactive web grid)
👉 Shiny for Python
Lets you:
Run SQL queries
Display results in a data grid UI
Can be extended to support editing + write-back

Supports “run SQL queries and display results in an interactive data grid”

Why it’s interesting
You can build:
A multi-table joined view
Editable UI (with some coding)
Works as a local or hosted web app

👉 This is one of the closest FLOSS ways to build your own “Access-like UI”

🟢 3. FreeSimpleSQL (lightweight GUI)
👉 FreeSimpleSQL
SQLite-focused
Has:
Query runner
Dynamic table view

“Dynamic data table… handles complex queries, aggregations, and joins”

Limitation
Not truly editable across joins
More of a viewer + query tool
🟡 4. Build-your-own (very common approach)

This is what most Python users actually do:

Stack:
SQLAlchemy / psycopg2 → run query
pandas → hold data
UI layer:
qgrid (Jupyter)
PyQt table widget
web grid (AG Grid, DataTables)
Why?

Because:

Python GUI toolkits don’t have a native “editable relational grid” component
