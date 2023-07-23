from seller import Seller

import psycopg2

from config import read_config, test_connect
from messages import *

"""
    Splits given command string by spaces and trims each token.
    Returns token list.
"""
def tokenize_command(command):
    tokens = command.split(" ")
    return [t.strip() for t in tokens]

class Mp2Client:
    def __init__(self, config_filename):
        self.db_conn_params = read_config(filename=config_filename, section="postgresql")
        self.conn = None

    """
        Connects to PostgreSQL database and returns connection object.
    """
    def connect(self):
        self.conn = psycopg2.connect(**self.db_conn_params)
        self.conn.autocommit = False

    """
        Disconnects from PostgreSQL database.
    """
    def disconnect(self):
        # self.conn.close()
        pass

    """
        Prints list of available commands of the software.
    """
    def help(self):
        # prints the choices for commands and parameters
        print("\n*** Please enter one of the following commands ***")
        print("> help")
        print("> sign_up <seller_id> <subscriber_key> <zip_code> <city> <state> <plan_id>")
        print("> sign_in <seller_id> <subscriber_key>")
        print("> sign_out")
        print("> show_plans")
        print("> show_subscription")
        print("> change_stock <product_id> <add or remove> <amount>")
        print("> show_quota")
        print("> subscribe <plan_id>")
        print("> ship <product_id_1> <product_id_2> <product_id_3> ... <product_id_n>")
        print("> calc_gross")
        print("> show_cart <customer_id>")
        print("> change_cart <customer_id> <product_id> <seller_id> <add or remove> <amount>")
        print("> purchase_cart <customer_id>")
        print("> quit")
    
    """
        Saves seller with given details.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    """
    def sign_up(self, seller_id, sub_key, zip, city, state, plan_id):
        try:
            # Create a cursor object
            with self.conn.cursor() as cur:

                # SQL query to check if a seller with the same seller_id already exists
                check_query = "SELECT * FROM sellers WHERE seller_id = %s"
                cur.execute(check_query, (seller_id,))
                result = cur.fetchone()

                # If a seller with the same seller_id already exists, return an error message
                if result is not None:
                    return False, CMD_EXECUTION_FAILED
                
                # Insert the new seller into the sellers table
                insert_sellers_query = """
                INSERT INTO sellers (seller_id, seller_zip_code_prefix, seller_city, seller_state)
                VALUES (%s, %s, %s, %s)
                """
                cur.execute(insert_sellers_query, (seller_id, zip, city, state))

                # Insert the new seller's subscription info into the seller_subscription table
                insert_subscription_query = """
                INSERT INTO seller_subscription (seller_id, subscriber_key, session_count, plan_id)
                VALUES (%s, %s, 0, %s)
                """
                cur.execute(insert_subscription_query, (seller_id, sub_key, plan_id))


                # Commit the transaction
                self.conn.commit()

                # Return success message
                return True, CMD_EXECUTION_SUCCESS

        except (Exception, psycopg2.DatabaseError) as error:
            self.conn.rollback()  # Rollback any changes made before the exception was raised
            return False, CMD_EXECUTION_FAILED
        
        finally:
            if self.conn is not None:
                cur.close()

    """
        Retrieves seller information if seller_id and subscriber_key is correct and seller's session_count < max_parallel_sessions.
        - Return type is a tuple, 1st element is a seller object and 2nd element is the response message from messages.py.
        - If seller_id or subscriber_key is wrong, return tuple (None, USER_SIGNIN_FAILED).
        - If session_count < max_parallel_sessions, commit changes (increment session_count) and return tuple (seller, CMD_EXECUTION_SUCCESS).
        - If session_count >= max_parallel_sessions, return tuple (None, USER_ALL_SESSIONS_ARE_USED).
        - If any exception occurs; rollback, do nothing on the database and return tuple (None, USER_SIGNIN_FAILED).
    """
    def sign_in(self, seller_id, sub_key):
        try:
            if self.conn is None:
                self.conn = self.connect(self)
            cur = self.conn.cursor()

            # Check if a seller with the same seller_id and subscriber_key exists
            check_query = """
            SELECT * FROM seller_subscription 
            WHERE seller_id = %s AND subscriber_key = %s
            """
            cur.execute(check_query, (seller_id, sub_key))
            result = cur.fetchone()

            # If a seller with the same seller_id and subscriber_key does not exist, return an error message
            if result is None:
                return None, USER_SIGNIN_FAILED

            # Get the current session count and plan id for the seller
            session_count = result[2]
            plan_id = result[3]


            # Get the max_parallel_sessions for the plan
            plan_query = "SELECT max_parallel_sessions FROM subscription_plans WHERE plan_id = %s"
            cur.execute(plan_query, (plan_id,))
            plan_result = cur.fetchone()

            if(plan_result == None):
                plan_result = 1
            
            max_parallel_sessions = plan_result[0]

            # If session_count < max_parallel_sessions, increment session_count and sign in the seller
            if session_count < max_parallel_sessions:
                session_count += 1
                # Update the session_count in the seller_subscription table
                update_query = """
                UPDATE seller_subscription 
                SET session_count = %s 
                WHERE seller_id = %s
                """
                cur.execute(update_query, (session_count, seller_id))

                # Commit the transaction
                self.conn.commit()
                cur.close()

                # Return success message
                return seller_id, CMD_EXECUTION_SUCCESS
            else:
                # If session_count >= max_parallel_sessions, return an error message
                return None, USER_ALL_SESSIONS_ARE_USED

        except (Exception, psycopg2.DatabaseError) as error:
            print(error)

            # If any exception occurs, rollback, do nothing on the database and return an error message
            if self.conn is not None:
                self.conn.rollback()
            return None, USER_SIGNIN_FAILED

        finally:
            if self.conn is not None:
                cur.close()


    """
        Signs out from given seller's account.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - Decrement session_count of the seller in the database.
        - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    """
    def sign_out(self, seller):
        try:
            if self.conn is None:
                self.conn = self.connect(self)
            cur = self.conn.cursor()

            # Check if a seller with the same seller_id exists and get the current session count
            check_query = """
            SELECT session_count FROM seller_subscription 
            WHERE seller_id = %s
            """
            cur.execute(check_query, (seller,))
            result = cur.fetchone()

            # If a seller with the same seller_id does not exist, return an error message
            if result is None:
                return False, CMD_EXECUTION_FAILED

            # Get the current session count for the seller
            session_count = result[0]

            # If session_count > 0, decrement session_count and sign out the seller
            if session_count > 0:
                session_count -= 1

                # Update the session_count in the seller_subscription table
                update_query = """
                UPDATE seller_subscription 
                SET session_count = %s 
                WHERE seller_id = %s
                """
                cur.execute(update_query, (session_count, seller))

                # Commit the transaction
                self.conn.commit()
                cur.close()

                # Return success message
                return True, CMD_EXECUTION_SUCCESS
            else:
                # If session_count <= 0, return an error message
                return False, CMD_EXECUTION_FAILED

        except (Exception, psycopg2.DatabaseError) as error:
            print(error)

            # If any exception occurs, rollback, do nothing on the database and return an error message
            if self.conn is not None:
                self.conn.rollback()
            return False, CMD_EXECUTION_FAILED

        finally:
            if self.conn is not None:
                cur.close()


    """
        Quits from program.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - Remember to sign authenticated user out first.
        - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    """
    def quit(self, seller):
        try:
            # Sign out the authenticated user first
            self.sign_out(seller)
            
            # If the operation is successful, commit changes
            if self.conn is not None:
                self.conn.commit()

            return True, CMD_EXECUTION_SUCCESS

        except (Exception, psycopg2.DatabaseError) as error:
            print(error)

            # If any exception occurs, rollback, do nothing on the database and return an error message
            if self.conn is not None:
                self.conn.rollback()
            return False, CMD_EXECUTION_FAILED


    """
        Retrieves all available plans and prints them.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful; print available plans and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
        
        Output should be like:
        #|Name|Max Sessions|Max Stocks Per Product
        1|Basic|2|4
        2|Advanced|4|8
        3|Premium|6|12
    """
    def show_plans(self):
        try:
            if self.conn is None:
                self.conn = self.connect(self)
            cur = self.conn.cursor()

            # Fetch all subscription plans from the table
            select_query = """
            SELECT * FROM subscription_plans
            """
            cur.execute(select_query)
            plans = cur.fetchall()

            # If plans exist, print them
            if plans:
                print("#|Name|Max Sessions|Max Stocks Per Product")
                for plan in plans:
                    print(f"{plan[0]}|{plan[1]}|{plan[2]}|{plan[3]}")
                
                # Commit the transaction
                self.conn.commit()
                cur.close()

                # Return success message
                return True, CMD_EXECUTION_SUCCESS
            else:
                # If no plans exist, return an error message
                return False, CMD_EXECUTION_FAILED

        except (Exception, psycopg2.DatabaseError) as error:
            print(error)

            # If any exception occurs, rollback, do nothing on the database and return an error message
            if self.conn is not None:
                self.conn.rollback()
            return False, CMD_EXECUTION_FAILED

        finally:
            if self.conn is not None:
                cur.close()
    
    def show_subscription(self, seller):
        try:
            if self.conn is None:
                self.conn = self.connect(self)
            cur = self.conn.cursor()

            # Fetch the subscription plan of the seller from the database
            select_query = """
            SELECT sp.* FROM subscription_plans sp 
            JOIN seller_subscription ss ON sp.plan_id = ss.plan_id 
            WHERE ss.seller_id = %s
            """
            cur.execute(select_query, (seller,))
            subscription = cur.fetchone()

            # If a subscription exists, print it
            if subscription:
                print("#|Name|Max Sessions|Max Stocks Per Product")
                print(f"{subscription[0]}|{subscription[1]}|{subscription[2]}|{subscription[3]}")

                # Commit the transaction
                self.conn.commit()
                cur.close()

                # Return success message
                return True, CMD_EXECUTION_SUCCESS
            else:
                # If no subscription exists for the seller, return an error message
                return False, CMD_EXECUTION_FAILED

        except (Exception, psycopg2.DatabaseError) as error:
            print(error)

            # If any exception occurs, rollback, do nothing on the database and return an error message
            if self.conn is not None:
                self.conn.rollback()
            return False, CMD_EXECUTION_FAILED

        finally:
            if self.conn is not None:
                cur.close()
    
    """
        Change stock count of a product.
        - Return type is a tuple, 1st element is a seller object and 2nd element is the response message from messages.py.
        - If target product does not exist on the database, return tuple (False, PRODUCT_NOT_FOUND).
        - If target stock count > current plan's max_stock_per_product, return tuple (False, QUOTA_LIMIT_REACHED).
        - If the operation is successful, commit changes and return tuple (seller, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    """
    def change_stock(self, seller, product_id, change_amount):
        try:
            if self.conn is None:
                self.conn = self.connect(self)
            cur = self.conn.cursor()

            # Fetch current stock and subscription plan of the seller
            select_query = """
            SELECT s.stock_count, sp.max_stock_per_product
            FROM seller_stocks s
            JOIN seller_subscription ss ON s.seller_id = ss.seller_id
            JOIN subscription_plans sp ON ss.plan_id = sp.plan_id
            WHERE s.seller_id = %s AND s.product_id = %s
            """
            cur.execute(select_query, (seller, product_id))
            result = cur.fetchone()
            if result is None:
                return False, PRODUCT_NOT_FOUND

            current_stock, max_stock = result
            new_stock = current_stock

            new_stock += change_amount
            if new_stock > max_stock:
                return False, QUOTA_LIMIT_REACHED

            # Update the stock in the database
            update_query = """
            UPDATE seller_stocks
            SET stock_count = %s
            WHERE seller_id = %s AND product_id = %s
            """

            cur.execute(update_query, (new_stock, seller, product_id))

            # Commit the transaction
            self.conn.commit()
            cur.close()

            # Return success message
            return True, CMD_EXECUTION_SUCCESS

        except (Exception, psycopg2.DatabaseError) as error:
            print(error)

            # If any exception occurs, rollback, do nothing on the database and return an error message
            if self.conn is not None:
                self.conn.rollback()
            return False, CMD_EXECUTION_FAILED

        finally:
            if self.conn is not None:
                cur.close()


    """
        Retrieves authenticated seller's remaining quota for stocks and prints it. 
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful; print the authenticated seller's quota and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).

        If the seller is subscribed to a plan with max_stock_per_product = 12 and
        the current stock for product 92bf5d2084dfbcb57d9db7838bac5cd0 is 10, then output should be like:
        
        Product Id|Remaining Quota
        92bf5d2084dfbcb57d9db7838bac5cd0|2

        If the seller does not have a stock, print 'Quota limit is not activated yet.'
    """
    def show_quota(self, seller):
        try:
            if self.conn is None:
                self.conn = self.connect(self)
            cur = self.conn.cursor()

            # Fetch the max stock per product of the seller's subscription
            max_stock_query = """
            SELECT sp.max_stock_per_product
            FROM seller_subscription ss
            JOIN subscription_plans sp ON ss.plan_id = sp.plan_id
            WHERE ss.seller_id = %s
            """
            cur.execute(max_stock_query, (seller,))
            max_stock = cur.fetchone()
            if max_stock is None:
                return False, SUBSCRIBE_PLAN_NOT_FOUND

            # Fetch the current stock of each product the seller has
            stock_query = """
            SELECT product_id, stock_count
            FROM seller_stocks
            WHERE seller_id = %s
            """
            cur.execute(stock_query, (seller,))
            stocks = cur.fetchall()
            
            if len(stocks) == 0:
                return False, QUOTA_INACTIVE

            # Calculate remaining quota for each product and print
            print("Product Id|Remaining Quota")
            for product_id, current_stock in stocks:
                remaining_quota = max_stock[0] - current_stock
                print(f"{product_id}|{remaining_quota}")

            cur.close()

            # Return success message
            return True, CMD_EXECUTION_SUCCESS

        except (Exception, psycopg2.DatabaseError) as error:
            print(error)

            # If any exception occurs, rollback, do nothing on the database and return an error message
            if self.conn is not None:
                self.conn.rollback()
            return False, CMD_EXECUTION_FAILED

        finally:
            if self.conn is not None:
                cur.close()


    """
        Subscribe authenticated seller to new plan.
        - Return type is a tuple, 1st element is a seller object and 2nd element is the response message from messages.py.
        - If target plan does not exist on the database, return tuple (None, PRODUCT_NOT_FOUND).
        - If the new plan's max_parallel_sessions < current plan's max_parallel_sessions, return tuple (None, SUBSCRIBE_MAX_PARALLEL_SESSIONS_UNAVAILABLE).
        - If the operation is successful, commit changes and return tuple (seller, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (None, CMD_EXECUTION_FAILED).
    """
    def subscribe(self, seller, plan_id):
        try:
            if self.conn is None:
                self.conn = self.connect(self)
            cur = self.conn.cursor()

            # Fetch details of the new plan
            select_plan_query = """
            SELECT plan_id, max_parallel_sessions
            FROM subscription_plans
            WHERE plan_id = %s
            """
            cur.execute(select_plan_query, (plan_id,))
            new_plan = cur.fetchone()
            if new_plan is None:
                return None, SUBSCRIBE_PLAN_NOT_FOUND

            # Fetch current subscription details of the seller
            select_subscription_query = """
            SELECT ss.plan_id, sp.max_parallel_sessions
            FROM seller_subscription ss
            JOIN subscription_plans sp ON ss.plan_id = sp.plan_id
            WHERE ss.seller_id = %s
            """
            cur.execute(select_subscription_query, (seller,))
            current_subscription = cur.fetchone()

            # Check if the new plan allows at least as many parallel sessions as the current plan
            if new_plan[1] < current_subscription[1]:
                return None, SUBSCRIBE_MAX_PARALLEL_SESSIONS_UNAVAILABLE

            # Update the seller's subscription
            update_subscription_query = """
            UPDATE seller_subscription
            SET plan_id = %s
            WHERE seller_id = %s
            """
            cur.execute(update_subscription_query, (new_plan[0], seller))

            # Commit the transaction
            self.conn.commit()
            cur.close()

            # Return success message
            return seller, CMD_EXECUTION_SUCCESS

        except (Exception, psycopg2.DatabaseError) as error:
            print(error)

            # If any exception occurs, rollback, do nothing on the database and return an error message
            if self.conn is not None:
                self.conn.rollback()
            return None, CMD_EXECUTION_FAILED

        finally:
            if self.conn is not None:
                cur.close()


    """
        Change stock amounts for multiple distinct products.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If everything is OK and the operation is successful, return (True, CMD_EXECUTION_SUCCESS).
        - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any one of the product ids is incorrect; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
        - If any one of the products is not in the stock; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    """
    def ship(self, seller, product_ids):
        try:
            if self.conn is None:
                self.conn = self.connect(self)
            cur = self.conn.cursor()

            for product_id in product_ids:
                # Check if the product exists and fetch current stock
                select_query = """
                SELECT stock_count
                FROM seller_stocks
                WHERE seller_id = %s AND product_id = %s
                """
                cur.execute(select_query, (seller, product_id))
                result = cur.fetchone()
                
                # If the product doesn't exist, rollback and return error message
                if result is None:
                    self.conn.rollback()
                    return False, CMD_EXECUTION_FAILED

                current_stock = result[0]

                # If the product is not in stock, rollback and return error message
                if current_stock <= 0:
                    self.conn.rollback()
                    return False, STOCK_UNAVAILABLE

                # Update the stock
                new_stock = current_stock - 1
                update_query = """
                UPDATE seller_stocks
                SET stock_count = %s
                WHERE seller_id = %s AND product_id = %s
                """
                cur.execute(update_query, (new_stock, seller, product_id))

            # If everything is okay, commit the transaction
            self.conn.commit()

            # Return success message
            return True, CMD_EXECUTION_SUCCESS

        except (Exception, psycopg2.DatabaseError) as error:
            print(error)

            # If any exception occurs, rollback and return error message
            if self.conn is not None:
                self.conn.rollback()
            return False, CMD_EXECUTION_FAILED

        finally:
            if self.conn is not None:
                cur.close()

    

    """
        Retrieves the gross income per product category for every month.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful; print the results and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
        
        Output should be like:
        Gross Income|Year|Month
        123.45|2018|1
        67.8|2018|2
    """
    def calc_gross(self, seller):
        try:
            if self.conn is None:
                self.conn = self.connect(self)
            cur = self.conn.cursor()

            # Query to calculate the gross income per month
            gross_query = """
            SELECT SUM(oi.price) AS Gross_Income, 
                EXTRACT(YEAR FROM o.order_purchase_timestamp) AS Year, 
                EXTRACT(MONTH FROM o.order_purchase_timestamp) AS Month
            FROM orders o
            JOIN order_items oi ON o.order_id = oi.order_id
            WHERE oi.seller_id = %s
            GROUP BY Year, Month
            ORDER BY Year, Month
            """
            cur.execute(gross_query, (seller,))
            gross_incomes = cur.fetchall()
            
            if len(gross_incomes) == 0:
                print("Gross Income: 0")
                return True, CMD_EXECUTION_SUCCESS
            
            print("Gross Income|Year|Month")
            for gross_income, year, month in gross_incomes:
                print(f"{gross_income:.2f}|{int(year)}|{int(month)}")

            # Return success message
            return True, CMD_EXECUTION_SUCCESS

        except (Exception, psycopg2.DatabaseError) as error:
            print(error)

            # If any exception occurs, return error message
            return False, CMD_EXECUTION_FAILED

        finally:
            if self.conn is not None:
                cur.close()


    """
        Retrieves items on the customer's shopping cart
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful; print items on the cart and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
        
        Output should be like:
        Seller Id|Product Id|Amount
        dd7ddc04e1b6c2c614352b383efe2d36|e5f2d52b802189ee658865ca93d83a8f|2
        5b51032eddd242adc84c38acab88f23d|c777355d18b72b67abbeef9df44fd0fd|3
        df560393f3a51e74553ab94004ba5c87|ac6c3623068f30de03045865e4e10089|1
    """
    def show_cart(self, customer_id):
        try:
            # connect to the PostgreSQL database
            if self.conn is None:
                self.conn = self.connect(self)
            cur = self.conn.cursor()

            # prepare and execute the SQL statement
            cur.execute("""
                SELECT seller_id, product_id, amount
                FROM customer_carts
                WHERE customer_id = %s
            """, (customer_id,))

            # fetch all the results
            items = cur.fetchall()

            if len(items) == 0:
                return False, CUSTOMER_NOT_FOUND

            print("Seller Id|Product Id|Amount")
            for item in items:
                print("|".join(map(str, item)))

            return True, CMD_EXECUTION_SUCCESS

        except (Exception, psycopg2.DatabaseError) as error:
            # if any exception occurs
            return False, CMD_EXECUTION_FAILED

        finally:
            if self.conn is not None:
                cur.close()

        
    """
        Change count of items in shopping cart
        - Return type is a tuple, 1st element is a seller object and 2nd element is the response message from messages.py.
        - If customer does not exist on the database, return tuple (False, CUSTOMER_NOT_FOUND).
        - If target product does not exist on the database, return tuple (False, PRODUCT_NOT_FOUND).
        - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
        - Consider stocks of sellers when you add items to the cart.
        customer_id 06b8999e2fba1a1fbc88172c00ba8bc7
        product_id 1e9e8ef04dbcff4541ed26657ea517e5
    """
    def change_cart(self, customer_id, product_id, seller_id, change_amount):
        cur = None
        try:
            if self.conn is None or self.conn.closed:
                self.conn = self.connect(self)
            cur = self.conn.cursor()

            # Check if customer exists
            cur.execute("SELECT * FROM customers WHERE customer_id = %s", (customer_id,))
            if cur.fetchone() is None:
                return False, CUSTOMER_NOT_FOUND

            # Check if product exists
            cur.execute("SELECT * FROM products WHERE product_id = %s", (product_id,))
            if cur.fetchone() is None:
                return False, PRODUCT_NOT_FOUND

            # Check if the item is in the cart
            cur.execute("SELECT amount FROM customer_carts WHERE customer_id = %s AND product_id = %s AND seller_id = %s", (customer_id, product_id, seller_id))
            cart_entry = cur.fetchone()

            if cart_entry is None and change_amount < 0:
                return False, CMD_EXECUTION_FAILED  # You can't remove an item that isn't in the cart

            # If the item is not in the cart and we want to add it
            if cart_entry is None and change_amount > 0:
                # Check if enough stock is available
                cur.execute("SELECT stock_count FROM seller_stocks WHERE product_id = %s AND seller_id = %s", (product_id, seller_id))
                stock_entry = cur.fetchone()
                if stock_entry is None or stock_entry[0] < change_amount:
                    return False, STOCK_UNAVAILABLE

                # Add item to the cart
                cur.execute("INSERT INTO customer_carts(customer_id, product_id, seller_id, amount) VALUES (%s, %s, %s, %s)",
                            (customer_id, product_id, seller_id, change_amount))
            else:
                # If the item is already in the cart
                new_amount = cart_entry[0] + change_amount

                # If we want to remove more items than available in the cart
                if new_amount < 0:
                    return False, CMD_EXECUTION_FAILED

                # If we want to add items, check if enough stock is available
                if change_amount > 0:
                    cur.execute("SELECT stock_count FROM seller_stocks WHERE product_id = %s AND seller_id = %s", (product_id, seller_id))
                    stock_entry = cur.fetchone()
                    cur.execute("SELECT amount FROM customer_carts WHERE product_id = %s AND seller_id = %s AND customer_id = %s", (product_id, seller_id, customer_id))
                    card_amount = cur.fetchone()

                    if stock_entry is None or stock_entry[0] < (change_amount + card_amount[0]):
                        return False, STOCK_UNAVAILABLE

                # If the new amount is 0, remove the item from the cart
                if new_amount == 0:
                    cur.execute("DELETE FROM customer_carts WHERE customer_id = %s AND product_id = %s AND seller_id = %s",
                                (customer_id, product_id, seller_id))
                else:
                    # Update the amount in the cart
                    cur.execute("UPDATE customer_carts SET amount = %s WHERE customer_id = %s AND product_id = %s AND seller_id = %s",
                                (new_amount, customer_id, product_id, seller_id))

            print("OK")
            self.conn.commit()
            return True, CMD_EXECUTION_SUCCESS

        except (Exception, psycopg2.DatabaseError) as error:
            print(error)
            if self.conn is not None:
                self.conn.rollback()
            return False, CMD_EXECUTION_FAILED

        finally:
            if cur is not None:
                cur.close()

    
    """
        Purchases items on the cart
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful; return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
        
        Actions:
        - Change stocks on seller_stocks table
        - Remove entries from customer_carts table
        - Add entries to order_items table
        - Add single entry to order table
        customer_id 06b8999e2fba1a1fbc88172c00ba8bc7
        product_id 1e9e8ef04dbcff4541ed26657ea517e5
    """
    def purchase_cart(self, customer_id):
        cur = None
        try:
            if self.conn is None or self.conn.closed:
                self.conn = self.connect(self)
            cur = self.conn.cursor()

            # Check if customer exists
            cur.execute("SELECT * FROM customers WHERE customer_id = %s", (customer_id,))
            if cur.fetchone() is None:
                return False, CMD_EXECUTION_FAILED

            # Fetch all items in customer's cart
            cur.execute("SELECT product_id, seller_id, amount FROM customer_carts WHERE customer_id = %s", (customer_id,))
            cart_items = cur.fetchall()
            if not cart_items:
                return False, CMD_EXECUTION_FAILED

            # Begin transaction
            cur.execute("BEGIN")

            # Create order in orders table
            cur.execute("INSERT INTO orders(customer_id, order_purchase_timestamp) VALUES(%s, CURRENT_TIMESTAMP) RETURNING order_id", (customer_id,))
            order_id = cur.fetchone()[0]

            # Process each cart item
            for item in cart_items:
                product_id, seller_id, amount = item

                # Check stock availability
                cur.execute("SELECT stock_count FROM seller_stocks WHERE product_id = %s AND seller_id = %s", (product_id, seller_id))
                stock_entry = cur.fetchone()
                if stock_entry is None or stock_entry[0] < amount:
                    # Roll back transaction if not enough stock
                    cur.execute("ROLLBACK")
                    return False, STOCK_UNAVAILABLE

                # Reduce stock count
                cur.execute("UPDATE seller_stocks SET stock_count = stock_count - %s WHERE product_id = %s AND seller_id = %s", (amount, product_id, seller_id))

                # Add entry to order_items table
                cur.execute("INSERT INTO order_items(order_id, product_id, seller_id) VALUES(%s, %s, %s)", (order_id, product_id, seller_id))

            # Remove items from customer's cart
            cur.execute("DELETE FROM customer_carts WHERE customer_id = %s", (customer_id,))

            # Commit transaction
            cur.execute("COMMIT")

            print("OK")
            self.conn.commit()
            return True, CMD_EXECUTION_SUCCESS

        except (Exception, psycopg2.DatabaseError) as error:
            print(error)
            if self.conn is not None:
                self.conn.rollback()
            return False, CMD_EXECUTION_FAILED

        finally:
            if cur is not None:
                cur.close()

