WITH monthly_freight AS (
    SELECT
        seller_id,
        EXTRACT(MONTH FROM shipping_limit_date) AS month,
        SUM(freight_value) AS monthly_freight_value
    FROM
        order_items
    WHERE
        EXTRACT(YEAR FROM shipping_limit_date) = 2018
    GROUP BY
        seller_id,
        EXTRACT(MONTH FROM shipping_limit_date)
),
yearly_freight AS (
    SELECT
        seller_id,
        SUM(freight_value) AS yearly_freight_value
    FROM
        order_items
    WHERE
        EXTRACT(YEAR FROM shipping_limit_date) = 2018
    GROUP BY
        seller_id
),
total_freight AS (
    SELECT
        SUM(freight_value) AS total_freight_value
    FROM
        order_items
    WHERE
        EXTRACT(YEAR FROM shipping_limit_date) = 2018
),
monthly_all_sellers AS (
    SELECT
        NULL AS seller_id,
        EXTRACT(MONTH FROM shipping_limit_date) AS month,
        SUM(freight_value) AS monthly_freight_value
    FROM
        order_items
    WHERE
        EXTRACT(YEAR FROM shipping_limit_date) = 2018
    GROUP BY
        EXTRACT(MONTH FROM shipping_limit_date)
)
SELECT
    seller_id,
    month,
    monthly_freight_value AS freight_value
FROM
    monthly_freight
UNION ALL
SELECT
    seller_id,
    NULL AS month,
    yearly_freight_value AS freight_value
FROM
    yearly_freight
UNION ALL
SELECT
    seller_id,
    month,
    monthly_freight_value AS freight_value
FROM
    monthly_all_sellers
UNION ALL
SELECT
    NULL AS seller_id,
    NULL AS month,
    total_freight_value AS freight_value
FROM
    total_freight
ORDER BY
    seller_id,
    month;
