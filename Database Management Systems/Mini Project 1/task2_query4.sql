SELECT oi.seller_id, AVG(o_r.review_score) AS avg_rating, COUNT(o_r.review_id) AS review_count
FROM order_reviews o_r
	JOIN orders o ON o_r.order_id = o.order_id
	JOIN customers oc ON o.customer_id = oc.customer_id
	JOIN order_items oi ON oi.order_id = o.order_id
	JOIN products p ON p.product_id = oi.product_id
	JOIN sellers s ON s.seller_id = oi.seller_id
WHERE p.product_category_name = 'automotivo' 
	AND s.seller_city  = 'sao paulo' 
GROUP BY oi.seller_id 
HAVING COUNT(o_r.review_id) >= 10 
ORDER BY AVG(o_r.review_score) DESC 
LIMIT 5;


