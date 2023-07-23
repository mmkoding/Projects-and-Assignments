WITH product_sales AS (
    SELECT
        p.product_id,
        p.product_category_name,
        COUNT(oi.order_id) AS total_sales
    FROM
        products p
        JOIN order_items oi ON p.product_id = oi.product_id
    WHERE
        p.product_category_name <> ''
    GROUP BY
        p.product_id,
        p.product_category_name 
    HAVING
        COUNT(oi.order_id) >= 10
)
SELECT
    product_id,
    total_sales,
    product_category_name,
    RANK() OVER (PARTITION BY product_category_name ORDER BY total_sales DESC) AS sales_rank
FROM
    product_sales
ORDER BY
    product_category_name,
    sales_rank;
