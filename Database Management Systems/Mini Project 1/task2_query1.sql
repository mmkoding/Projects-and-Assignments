SELECT DISTINCT c.customer_city
FROM customers c
INNER JOIN orders o ON c.customer_id = o.customer_id
INNER JOIN order_payments op ON o.order_id = op.order_id
WHERE op.payment_type = 'voucher'
AND NOT EXISTS (
  SELECT 1
  FROM order_payments op2
  WHERE op2.order_id = o.order_id
  AND op2.payment_type <> 'voucher'
);