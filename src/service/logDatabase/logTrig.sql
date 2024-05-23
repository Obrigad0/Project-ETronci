\c :log_dbname

-- drop all functions and triggers
DO $$
DECLARE
function_name TEXT;
BEGIN
FOR function_name IN
SELECT p.proname
FROM pg_proc p
         INNER JOIN pg_namespace n ON p.pronamespace = n.oid
WHERE n.nspname = 'public'
    LOOP
        EXECUTE 'DROP FUNCTION IF EXISTS ' || function_name || ' CASCADE';
END LOOP;
END $$;

CREATE OR REPLACE FUNCTION consistent_file_descriptor() RETURNS TRIGGER AS $$
BEGIN
    IF EXISTS (
        SELECT 1
        FROM Client
        WHERE NEW.server_name = Client.server_name
        AND NEW.file_descriptor = Client.file_descriptor
        AND (Client.disconnection_instant IS NULL OR NEW.connection_instant <= Client.disconnection_instant)
    ) IS TRUE THEN
        RAISE EXCEPTION 'file descriptor connection not closed';
END IF;

RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER consistent_file_descriptor_trg
    BEFORE INSERT ON Client FOR EACH ROW EXECUTE FUNCTION consistent_file_descriptor();