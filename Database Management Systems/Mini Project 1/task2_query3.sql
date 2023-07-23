SELECT 
    p.product_category_name,
    COUNT(*) AS late_delivery_count
FROM 
    products p,
    order_items oi,
    orders o,
    customers c
WHERE
	p.product_id = oi.product_id
	AND oi.order_id = o.order_id
	AND o.customer_id = c.customer_id
    AND c.customer_city = 'rio de janeiro' 
    AND o.order_purchase_timestamp >= '2018-06-01' 
    AND o.order_purchase_timestamp < '2018-09-01' 
    AND o.order_delivered_customer_date > o.order_estimated_delivery_date
GROUP BY 
    p.product_category_name
ORDER BY 
    late_delivery_count DESC,
    p.product_category_name ASC;
