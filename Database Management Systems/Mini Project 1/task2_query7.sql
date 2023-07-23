SELECT
  p.product_id,
  AVG(r.review_score) AS review_avg,
  COUNT(r.review_score) AS review_count
FROM
  products p
  JOIN order_items oi ON p.product_id = oi.product_id
  JOIN orders o ON oi.order_id = o.order_id
  JOIN order_reviews r ON oi.order_id = r.order_id 
WHERE
  p.product_category_name = 'eletronicos'
GROUP BY
  p.product_id
HAVING
  COUNT(r.review_score) >= 5
ORDER BY
  review_avg DESC,
  review_count DESC
LIMIT 10;

