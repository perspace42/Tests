-- 1. Create the view
CREATE VIEW dbo.vw_Editable AS
SELECT
    m.id            AS Main_id,
    m.name          AS Main_name,
    m.desc          AS Main_desc,
    m.comments      AS Main_comments,
    s1.value        AS Sub_One_value,
    s2.value        AS Sub_Two_value,
    s3.value        AS Sub_Three_value
FROM Main_Table m
JOIN Sub_One_Table   s1 ON s1.id = m.id
JOIN Sub_Two_Table   s2 ON s2.id = m.id
JOIN Sub_Three_Table s3 ON s3.id = m.id;
GO
-- 2. INSTEAD OF trigger to route edits back to correct tables
CREATE TRIGGER trg_vw_Editable_Update
ON dbo.vw_Editable
INSTEAD OF UPDATE
AS
BEGIN
    -- Update Main_Table fields
    UPDATE m
    SET
        m.name     = i.Main_name,
        m.desc     = i.Main_desc,
        m.comments = i.Main_comments
    FROM Main_Table m
    JOIN inserted i ON m.id = i.Main_id;
    -- Update Sub_One_Table
    UPDATE s1
    SET s1.value = i.Sub_One_value
    FROM Sub_One_Table s1
    JOIN inserted i ON s1.id = i.Main_id;
    -- Update Sub_Two_Table
    UPDATE s2
    SET s2.value = i.Sub_Two_value
    FROM Sub_Two_Table s2
    JOIN inserted i ON s2.id = i.Main_id;
    -- Update Sub_Three_Table
    UPDATE s3
    SET s3.value = i.Sub_Three_value
    FROM Sub_Three_Table s3
    JOIN inserted i ON s3.id = i.Main_id;
END;
GO
-- 3. Use this in SSMS Edit Rows pane
SELECT * FROM dbo.vw_Editable;