WITH q1_sales AS (
  SELECT
    oi.seller_id,
    EXTRACT(MONTH FROM o.order_purchase_timestamp) AS month,
    COUNT(*) AS num_items
  FROM
    order_items oi
    JOIN orders o ON oi.order_id = o.order_id
  WHERE
    o.order_purchase_timestamp >= '2018-01-01' AND o.order_purchase_timestamp < '2018-04-01'
  GROUP BY
    oi.seller_id,
    EXTRACT(MONTH FROM o.order_purchase_timestamp)
), q1_earnings AS (
  SELECT
    oi.seller_id,
    SUM(oi.price) AS total_earned
  FROM
    order_items oi
    JOIN orders o ON oi.order_id = o.order_id
  WHERE
    o.order_purchase_timestamp >= '2018-01-01' AND o.order_purchase_timestamp < '2018-04-01'
  GROUP BY
    oi.seller_id
)
SELECT
  q1_earnings.seller_id,
  q1_earnings.total_earned
FROM
  q1_earnings
  JOIN q1_sales ON q1_earnings.seller_id = q1_sales.seller_id AND q1_sales.month = 1 AND q1_sales.num_items >= 50
  JOIN q1_sales q1_sales2 ON q1_earnings.seller_id = q1_sales2.seller_id AND q1_sales2.month = 2 AND q1_sales2.num_items >= 50
  JOIN q1_sales q1_sales3 ON q1_earnings.seller_id = q1_sales3.seller_id AND q1_sales3.month = 3 AND q1_sales3.num_items >= 50
ORDER BY
  total_earned DESC;
