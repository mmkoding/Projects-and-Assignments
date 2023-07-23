CREATE TABLE IF NOT EXISTS user_review_scores (
    product_id VARCHAR(32),
    avg_review_score NUMERIC(5, 2),
    reviews_count INTEGER,
    PRIMARY KEY (product_id),
    FOREIGN KEY (product_id) REFERENCES products (product_id)
);

INSERT INTO user_review_scores (product_id, avg_review_score, reviews_count)
SELECT
    oi.product_id,
    AVG(o_r.review_score) AS avg_review_score,
    COUNT(o_r.review_id) AS reviews_count
FROM
    order_items oi
    JOIN order_reviews o_r ON oi.order_id = o_r.order_id
GROUP BY
    oi.product_id;

   
CREATE OR REPLACE FUNCTION update_user_review_scores()
RETURNS TRIGGER AS $$
BEGIN
    UPDATE user_review_scores
    SET
        avg_review_score = (avg_review_score * reviews_count + NEW.review_score) / (reviews_count + 1),
        reviews_count = reviews_count + 1
    WHERE
        product_id = (SELECT product_id FROM order_items WHERE order_id = NEW.order_id);

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER update_review_scores
AFTER INSERT ON order_reviews
FOR EACH ROW
EXECUTE FUNCTION update_user_review_scores();

--%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%--

CREATE OR REPLACE FUNCTION check_seller_zip_code()
RETURNS TRIGGER AS $$
BEGIN
    IF NEW.seller_zip_code_prefix = '00000' THEN
        RAISE EXCEPTION 'Zip code of the seller CAN NOT be zero!';
    END IF;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER prevent_invalid_seller_zip_code
BEFORE INSERT ON sellers
FOR EACH ROW
EXECUTE FUNCTION check_seller_zip_code();

--%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%--

CREATE VIEW order_product_customer_review AS
SELECT
    o.order_id,
    oi.product_id,
    o.customer_id,
    r.review_score
FROM
    orders o
JOIN order_items oi ON o.order_id = oi.order_id
JOIN order_reviews r ON o.order_id = r.order_id;

CREATE MATERIALIZED VIEW order_product_customer_review_materialized AS
SELECT
    o.order_id,
    oi.product_id,
    o.customer_id,
    r.review_score
FROM
    orders o
JOIN order_items oi ON o.order_id = oi.order_id
JOIN order_reviews r ON o.order_id = r.order_id;


--The difference between a standard view and a materialized view is that a standard view is just a 
--saved query that gets executed every time we query the view, while a materialized view stores 
--the results of the query when we create or refresh the view.

--A materialized view can significantly improve the performance of the queries that use it, as 
--the results are already precomputed and stored. However, materialized views require more 
--storage space and need to be refreshed to reflect changes in the underlying data. On the 
--other hand, a standard view always shows the most recent data but may have a longer run 
--time due to the need to execute the query every time it's accessed.





