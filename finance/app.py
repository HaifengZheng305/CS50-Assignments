import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_cash = db.execute("select cash from users where ID = ?", session["user_id"])
    user = db.execute("select symbol, sum(amount), `buy/sell` from trans where user_ID = (?) group by symbol", session["user_id"])
    index = []
    total_stock = 0
    for row in user:
        if row["sum(amount)"] == 0:
            db.execute("DELETE FROM trans where symbol = ?", row["symbol"])
        temp= {}
        temp["symbol"] = row["symbol"]
        temp["name"] = lookup(row["symbol"])["name"]
        temp["price"] = lookup(row["symbol"])["price"]
        temp["price_usd"] = usd(temp["price"])
        temp["amount"] = int(row["sum(amount)"])
        temp["total"] = temp["amount"] * temp["price"]
        temp["total_usd"] = usd(temp["total"])
        total_stock += temp["total"]
        index.append(temp)

    return render_template("index.html", cash = usd(user_cash[0]["cash"]), index = index, total = usd(total_stock + user_cash[0]["cash"]))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        symbol = request.form.get("symbol")
        amount = request.form.get("shares")
        user_cash = db.execute("select cash from users where id = ?", session["user_id"])
        stock = lookup(symbol.upper())
        date = datetime.now()

        if not symbol:
            return apology("Must Give Symbol")

        if not stock:
            return apology("No symbol Exist")

        try:
            int(amount)
        except:
            return apology("Share Not Allowed")

        total = stock["price"] * float(amount)
        uptd_cash = user_cash[0]["cash"] - total

        if int(amount) <= 0:
            return apology("Share Not Allowed")

        if user_cash[0]["cash"] < total:
            return apology("Not Enough Cash")

        db.execute("UPDATE users SET CASH = ? WHERE id = ?", uptd_cash, session["user_id"])

        db.execute("INSERT INTO trans(user_ID, symbol, amount, price, time, `buy/sell`) VALUES (?,?,?,?,?,?)", session["user_id"], symbol.upper(), amount, stock["price"], date, "buy" )

        return redirect("/")

    else:
        return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * FROM trans where user_ID = ?", session["user_id"])
    for row in history:
        row["price"] = usd(row["price"])
    return render_template("history.html", index = history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))




        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            print (len(rows))
            print (check_password_hash(rows[0]["hash"], request.form.get("password")))

            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Must Give Symbol")
        stock = lookup(symbol)

        if not stock:
            return apology("No symbol Exist")

        return render_template("quoted.html", name=stock["name"], price = usd(stock["price"]), symbol = stock["symbol"])

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":

        """Register user"""
        username = request.form.get("username")
        password = request.form.get("password")
        re_password = request.form.get("confirmation")
        """check if username is taken"""
        name = db.execute("SELECT * FROM users WHERE username =?", username)

        if not username:
            return apology("Must Give Username")

        if not password:
           return apology("Must Give Password")

        if not re_password:
            return apology("Must Give Comfirmation")

        if password != re_password:
            return apology("Password does not match")

        hash = generate_password_hash(password)

        try:
            db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, hash)
        except:
            return apology("Username already Exist")

        return render_template("register_completed.html")

    else:

        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        amount = request.form.get("shares")
        user_cash = db.execute("select cash from users where id = ?", session["user_id"])
        stock = lookup(symbol.upper())
        date = datetime.now()
        user = db.execute("select symbol, sum(amount), `buy/sell` from trans where user_ID = (?) group by symbol", session["user_id"])
        stockls = []
        stock_share = {}

        for rows in user:
            stock_share[rows["symbol"]] = int(rows["sum(amount)"])
        print (stock_share)

        for rows in user:
            stockls.append(rows["symbol"])

        if symbol not in stockls:
            return apology("Do not own stock")
        try:
            int(amount)
        except:
            return apology("Share Not Allowed")

        if int(amount) <= 0:
            return apology("Share Not Allowed")

        if stock_share[symbol] < int(amount):
            return apology("Share Not Allowed")


        uptd_cash = user_cash[0]["cash"] + (stock["price"] * int(amount))
        db.execute("UPDATE users SET CASH = ? WHERE id = ?", uptd_cash, session["user_id"])
        db.execute("INSERT INTO trans(user_ID, symbol, amount, price, time, `buy/sell`) VALUES (?,?,?,?,?,?)", session["user_id"], symbol.upper(), int(-1) * int(amount), stock["price"], date, "sell" )
        
        flash("SOLD")

        return redirect("/")
    else:
        user = db.execute("select symbol from trans where user_ID = (?) group by symbol", session["user_id"])
        return render_template ("sell.html", index = user)

