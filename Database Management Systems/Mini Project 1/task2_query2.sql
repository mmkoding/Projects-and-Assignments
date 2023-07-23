WITH curitiba_health_sellers AS (
  SELECT
    DISTINCT s.seller_id
  FROM
    sellers s
    JOIN order_items oi ON oi.seller_id = s.seller_id
    JOIN products p ON p.product_id = oi.product_id
  WHERE
    s.seller_city = 'curitiba'
    AND p.product_category_name = 'beleza_saude'
),
shipping_duration AS (
  SELECT
    chs.seller_id,
    o.order_id,
    o.order_delivered_carrier_date - o.order_purchase_timestamp AS duration
  FROM
    curitiba_health_sellers chs
    JOIN order_items oi ON oi.seller_id = chs.seller_id
    JOIN orders o ON o.order_id = oi.order_id
    JOIN products p ON p.product_id = oi.product_id
  WHERE
    p.product_category_name = 'beleza_saude'
)
SELECT
  seller_id,
  to_char(AVG(duration), 'DD "day" HH24:MI:SS') AS avg_duration
FROM
  shipping_duration
GROUP BY
  seller_id
HAVING
  AVG(duration) <= interval '2 days'
ORDER BY
  AVG(duration) DESC;

 




