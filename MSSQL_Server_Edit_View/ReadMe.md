# Editable Multi-Table View in SQL Server

## Goal

Create a single editable view across `Main_Table`, `Sub_One_Table`, `Sub_Two_Table`, and `Sub_Three_Table` such that changes made in SSMS's **Edit Top 200 Rows** grid (or via SQL `UPDATE`/`INSERT`/`DELETE`) are written back to the correct underlying tables.

**Yes — `INSTEAD OF` triggers are the correct approach for this.**

---

## Step 1 — Table Definitions (Reference)

| | |
|---|---|
| **What** | Define the four tables that hold your actual data. |
| **Why** | Everything else depends on this structure. The subtables use `Main_Table.id` as both their primary key and foreign key, enforcing a strict 1-to-1 relationship. This is what lets the view and triggers reliably route reads and writes to the right row in each table. |

```sql
CREATE TABLE dbo.Main_Table (
    id       INT PRIMARY KEY IDENTITY,
    name     NVARCHAR(255),
    [desc]   NVARCHAR(255),   -- [desc] quoted: reserved keyword
    comments NVARCHAR(255)
);

CREATE TABLE dbo.Sub_One_Table (
    id    INT PRIMARY KEY REFERENCES dbo.Main_Table(id),
    value NVARCHAR(255)
);

CREATE TABLE dbo.Sub_Two_Table (
    id    INT PRIMARY KEY REFERENCES dbo.Main_Table(id),
    value NVARCHAR(255)
);

CREATE TABLE dbo.Sub_Three_Table (
    id    INT PRIMARY KEY REFERENCES dbo.Main_Table(id),
    value NVARCHAR(255)
);
```

---

## Step 2 — Create the View

| | |
|---|---|
| **What** | Create a virtual table (`vw_Editable`) that presents columns from all four tables as a single flat row. |
| **Why** | SSMS's edit grid only enables cell editing when the `FROM` clause references a single object. Hiding the JOINs inside a view means SSMS sees one object and enables the grid. The view stores no data — it's a named query; all data lives in the underlying tables. |

```sql
CREATE VIEW dbo.vw_Editable AS
SELECT
    m.id            AS Main_id,
    m.name          AS Main_name,
    m.[desc]        AS Main_desc,
    m.comments      AS Main_comments,
    s1.value        AS Sub_One_value,
    s2.value        AS Sub_Two_value,
    s3.value        AS Sub_Three_value
FROM dbo.Main_Table m
LEFT JOIN dbo.Sub_One_Table   s1 ON s1.id = m.id
LEFT JOIN dbo.Sub_Two_Table   s2 ON s2.id = m.id
LEFT JOIN dbo.Sub_Three_Table s3 ON s3.id = m.id;
GO
```

> [!NOTE]
> `LEFT JOIN` is used so rows in `Main_Table` appear even when subtable rows don't exist yet. `INNER JOIN` would silently hide those rows.

---

## Step 3 — INSTEAD OF UPDATE Trigger

| | |
|---|---|
| **What** | Intercept any `UPDATE` sent to the view and manually route changes to the correct underlying tables. |
| **Why** | SQL Server cannot determine which table to write to when a view spans multiple tables — without this trigger, any edit fails. `INSTEAD OF` tells SQL Server to skip its own update logic and run your code instead. `MERGE` is used for subtables so a missing row is created rather than causing an error. |

```sql
CREATE TRIGGER dbo.trg_vw_Editable_Update
ON dbo.vw_Editable
INSTEAD OF UPDATE
AS
BEGIN
    SET NOCOUNT ON;
    BEGIN TRAN;
    BEGIN TRY

        UPDATE m
        SET
            m.name     = i.Main_name,
            m.[desc]   = i.Main_desc,
            m.comments = i.Main_comments
        FROM dbo.Main_Table m
        JOIN inserted i ON m.id = i.Main_id;

        -- Upsert Sub_One_Table (handles missing rows gracefully)
        MERGE dbo.Sub_One_Table AS t
        USING inserted AS i ON t.id = i.Main_id
        WHEN MATCHED THEN UPDATE SET t.value = i.Sub_One_value
        WHEN NOT MATCHED THEN INSERT (id, value) VALUES (i.Main_id, i.Sub_One_value);

        MERGE dbo.Sub_Two_Table AS t
        USING inserted AS i ON t.id = i.Main_id
        WHEN MATCHED THEN UPDATE SET t.value = i.Sub_Two_value
        WHEN NOT MATCHED THEN INSERT (id, value) VALUES (i.Main_id, i.Sub_Two_value);

        MERGE dbo.Sub_Three_Table AS t
        USING inserted AS i ON t.id = i.Main_id
        WHEN MATCHED THEN UPDATE SET t.value = i.Sub_Three_value
        WHEN NOT MATCHED THEN INSERT (id, value) VALUES (i.Main_id, i.Sub_Three_value);

        COMMIT;
    END TRY
    BEGIN CATCH
        ROLLBACK;
        THROW;
    END CATCH;
END;
GO
```

---

## Step 4 — INSTEAD OF INSERT Trigger

| | |
|---|---|
| **What** | Intercept any `INSERT` into the view and split the new row across all four tables in the correct order. |
| **Why** | SQL Server doesn't know which table gets which columns on a multi-table view. The parent (`Main_Table`) must be inserted first because it generates the `id` that subtable rows need as their foreign key. Children are inserted after, using that `id`. |

```sql
CREATE TRIGGER dbo.trg_vw_Editable_Insert
ON dbo.vw_Editable
INSTEAD OF INSERT
AS
BEGIN
    SET NOCOUNT ON;
    BEGIN TRAN;
    BEGIN TRY

        -- Insert parent first (generates the id)
        INSERT INTO dbo.Main_Table (name, [desc], comments)
        SELECT Main_name, Main_desc, Main_comments FROM inserted;

        -- Insert children using the newly generated id
        INSERT INTO dbo.Sub_One_Table (id, value)
        SELECT m.id, i.Sub_One_value
        FROM inserted i
        JOIN dbo.Main_Table m ON m.name = i.Main_name; -- match on name to get new id

        INSERT INTO dbo.Sub_Two_Table (id, value)
        SELECT m.id, i.Sub_Two_value
        FROM inserted i
        JOIN dbo.Main_Table m ON m.name = i.Main_name;

        INSERT INTO dbo.Sub_Three_Table (id, value)
        SELECT m.id, i.Sub_Three_value
        FROM inserted i
        JOIN dbo.Main_Table m ON m.name = i.Main_name;

        COMMIT;
    END TRY
    BEGIN CATCH
        ROLLBACK;
        THROW;
    END CATCH;
END;
GO
```

> [!IMPORTANT]
> The INSERT trigger matches the new `Main_Table` row back by `name` to retrieve its generated `id`. If `name` is not unique, use `SCOPE_IDENTITY()` for single-row inserts instead, or add a unique constraint to `name`.

---

## Step 5 — INSTEAD OF DELETE Trigger

| | |
|---|---|
| **What** | Intercept any `DELETE` from the view and remove corresponding rows from all four tables. |
| **Why** | Foreign key constraints require children to be deleted before the parent. Without this trigger, a delete from the view would fail because all three subtables reference `Main_Table.id`. The trigger deletes children first, then the parent, in the correct order. |

```sql
CREATE TRIGGER dbo.trg_vw_Editable_Delete
ON dbo.vw_Editable
INSTEAD OF DELETE
AS
BEGIN
    SET NOCOUNT ON;
    BEGIN TRAN;
    BEGIN TRY

        -- Delete children first (foreign key constraint order)
        DELETE s1 FROM dbo.Sub_One_Table   s1 JOIN deleted d ON s1.id = d.Main_id;
        DELETE s2 FROM dbo.Sub_Two_Table   s2 JOIN deleted d ON s2.id = d.Main_id;
        DELETE s3 FROM dbo.Sub_Three_Table s3 JOIN deleted d ON s3.id = d.Main_id;

        -- Then delete parent
        DELETE m FROM dbo.Main_Table m JOIN deleted d ON m.id = d.Main_id;

        COMMIT;
    END TRY
    BEGIN CATCH
        ROLLBACK;
        THROW;
    END CATCH;
END;
GO
```

---

## Step 6 — Use in SSMS

| | |
|---|---|
| **What** | Replace the auto-generated query in SSMS's Edit Rows SQL pane with a plain `SELECT` against the view. |
| **Why** | SSMS generates a `SELECT TOP 200` against a base table. Replacing it with `SELECT * FROM dbo.vw_Editable` makes SSMS treat the result as a single-object query, enabling grid editing. Any cell edit you make automatically fires the `INSTEAD OF UPDATE` trigger. |

In the **Edit Top 200 Rows** SQL pane, replace the generated query with:

```sql
SELECT * FROM dbo.vw_Editable;
```

SSMS sees a single object → enables grid editing → trigger routes all writes.

---

## Constraints & Gotchas

### 1. `desc` is a Reserved Keyword
Always quote it with square brackets everywhere.

```sql
-- ❌ Will error
SELECT desc FROM Main_Table;

-- ✅ Correct
SELECT [desc] FROM Main_Table;
```

---

### 2. INNER JOIN Hides Incomplete Rows
```sql
-- ❌ A Main_Table row with no Sub_One row disappears from the view
FROM Main_Table m INNER JOIN Sub_One_Table s1 ON s1.id = m.id

-- ✅ Row appears with NULL for Sub_One_value
FROM Main_Table m LEFT JOIN Sub_One_Table s1 ON s1.id = m.id
```

---

### 3. Must Include Primary Key in View
SSMS requires a unique key column to track which row to write back. `Main_id` must always be in the `SELECT` list.

```sql
-- ❌ SSMS cannot update — no key to identify the row
SELECT Main_name, Sub_One_value FROM dbo.vw_Editable;

-- ✅ Include the key
SELECT Main_id, Main_name, Sub_One_value FROM dbo.vw_Editable;
```

---

### 4. Partial Writes Without a Transaction
```sql
-- ❌ If Sub_Two update fails, Main and Sub_One are already written — data is now inconsistent
UPDATE Main_Table ...
UPDATE Sub_One_Table ...
UPDATE Sub_Two_Table ...  -- error here

-- ✅ Wrap everything in BEGIN TRAN / COMMIT / ROLLBACK (done in triggers above)
```

---

### 5. View Cannot Use ORDER BY, GROUP BY, DISTINCT, or UNION
These make the view non-updatable even with triggers.

```sql
-- ❌ Invalid in a view used with INSTEAD OF triggers
CREATE VIEW dbo.vw_Editable AS
SELECT DISTINCT m.id, ... FROM Main_Table m ...

-- ✅ Plain SELECT with JOINs only
```

---

### 6. SSMS May Show Some Cells as Read-Only
This is a cosmetic display decision by SSMS — it doesn't prevent your trigger from working. You can always write the `UPDATE` SQL manually in the query pane.
