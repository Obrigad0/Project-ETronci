\c :dbname



-- 1. trigger sulla tabella delivery

CREATE OR REPLACE FUNCTION check_courier_order_warehouse() RETURNS TRIGGER AS $$
BEGIN
    -- Verifica che il corriere stia accettando un ordine dalla stessa warehouse in cui lavora
    IF NOT EXISTS (
        SELECT 1
        FROM OrderedProduct op
        JOIN Product p ON op.product = p.id
        JOIN Courier c ON NEW.courier = c.id
        WHERE NEW.orderId = op.id
        AND p.warehouse = c.warehouse
    ) THEN
        RAISE EXCEPTION 'Courier and order must be from the same warehouse before accepting a delivery';
    END IF;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER check_courier_order_warehouse_trg
BEFORE INSERT ON Delivery
FOR EACH ROW
EXECUTE FUNCTION check_courier_order_warehouse();



-- 2. trigger sulla tabella review

CREATE OR REPLACE FUNCTION check_review_validity() RETURNS TRIGGER AS $$
BEGIN
    -- Verifica se il cliente ha effettivamente ordinato (e ricevuto) il prodotto
    IF NOT EXISTS (
        SELECT 1
        FROM OrderedProduct op
        JOIN Delivery d ON op.id = d.orderId
        WHERE op.customer = NEW.customer
        AND op.product = NEW.product
        AND d.status = 'delivered'
    ) THEN
        RAISE EXCEPTION 'Customer can only review products they have ordered and received';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER check_review_validity_trg
BEFORE INSERT ON Review
FOR EACH ROW
EXECUTE FUNCTION check_review_validity();

