WITH sao_paulo_sales AS (
  SELECT
    p.product_category_name,
    COUNT(o.order_id) AS past_order_count
  FROM
    products p
    JOIN order_items oi ON p.product_id = oi.product_id
    JOIN orders o ON oi.order_id = o.order_id
    JOIN customers c ON o.customer_id = c.customer_id
  WHERE
    c.customer_city = 'sao paulo'
  GROUP BY
    p.product_category_name
),
sellers_by_category AS (
  SELECT DISTINCT
    p.product_category_name,
    s.seller_city
  FROM
    products p
    JOIN order_items oi ON p.product_id = oi.product_id
    JOIN sellers s ON oi.seller_id = s.seller_id
)
SELECT
  sps.product_category_name,
  sps.past_order_count
FROM
  sao_paulo_sales sps
WHERE
  NOT EXISTS (
    SELECT
      1
    FROM
      sellers_by_category sbc
    WHERE
      sbc.product_category_name = sps.product_category_name
      AND sbc.seller_city = 'sao paulo'
  )
  AND sps.past_order_count >= 10
ORDER BY
  sps.product_category_name ASC;







 
