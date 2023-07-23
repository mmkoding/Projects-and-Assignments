WITH monthly_totals AS (
    SELECT
        c.customer_id,
        EXTRACT(YEAR FROM o.order_purchase_timestamp) AS year,
        EXTRACT(MONTH FROM o.order_purchase_timestamp) AS month,
        SUM(oi.price) AS total
    FROM
        orders o
        JOIN order_items oi ON o.order_id = oi.order_id
        JOIN customers c ON o.customer_id = c.customer_id
    GROUP BY
        c.customer_id,
        o.order_id,
        year,
        month
)
, max_monthly_totals AS (
    SELECT
        year,
        month,
        MAX(total) AS max_total
    FROM
        monthly_totals
    GROUP BY
        year,
        month
)
SELECT
    mt.customer_id,
    mt.year,
    mt.month,
    mt.total AS max_total
FROM
    monthly_totals mt
    JOIN max_monthly_totals mmt ON mt.year = mmt.year AND mt.month = mmt.month AND mt.total = mmt.max_total
ORDER BY
    mt.year ASC,
    mt.month ASC;
