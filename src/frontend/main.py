from flask import Flask, render_template, request, redirect, url_for
from jinja2 import Environment
import subprocess


def min_filter(values):
    return min(values)


env = Environment()
env.filters["min"] = min_filter

app = Flask(__name__, static_folder="../../html/static", template_folder="../../html/templates")

proc = subprocess.Popen("./../../cmake-build-debug/code", stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE, text=True)


def get_books():
    proc.stdin.write("show\n")
    proc.stdin.flush()
    n = proc.stdout.readline()
    n = int(n)
    books = []
    for i in range(n):
        tmp = proc.stdout.readline().split()
        tmp[3] = tmp[3].replace('|', ',')
        tmp[5] = int(tmp[5])
        books.append(tmp)
    return books


def get_status():
    proc.stdin.write("info\n")
    proc.stdin.flush()
    tmp = proc.stdout.readline().split()
    tmp[2] = int(tmp[2])
    return tmp


@app.route("/")
def main_page():
    login_status = bool(request.args.get("login_fail"))
    register_status = bool(request.args.get("register_fail"))
    return render_template("main.html", books=get_books(), login_fail=login_status,
                           register_fail=register_status)


@app.route("/home", methods=["get"])
def home_page():
    password_fail = request.args.get("password_fail") == "True"
    password_succeed = request.args.get("password_succeed") == "True"
    result = get_status()
    return render_template("login_main.html", books=get_books(), username=result[1],
                           privilege=result[2], password_fail=password_fail, password_succeed=password_succeed,
                           page=1)


@app.route("/login", methods=["post"])
def login_main_page():
    account = request.form.get("login_userid").strip()
    password = request.form.get("login_password").strip()
    proc.stdin.write("su " + account + ' ' + password + '\n')
    proc.stdin.flush()
    result = proc.stdout.readline().split()
    if result[0] == "succeed":
        return redirect(url_for("home_page"))
    else:
        return redirect(url_for("main_page", login_fail=True))


@app.route("/bookinfo", methods=["get"])
def show_bookinfo():
    page = int(request.args.get("page", 1))
    proc.stdin.write("info\n")
    proc.stdin.flush()
    result = proc.stdout.readline().split()
    result[2] = int(result[2])
    return render_template("login_main.html", books=get_books(), username=result[1],
                           privilege=result[2], password_fail=False, password_succeed=False, page=page)


@app.route("/password", methods=["post"])
def main_page_change_password():
    old_password = request.form.get("old_password").strip()
    new_password = request.form.get("new_password").strip()
    status=get_status()
    userid = status[0]
    proc.stdin.write("passwd " + userid + " " + old_password + " " + new_password + '\n')
    proc.stdin.flush()
    result = proc.stdout.readline().strip()
    if result == "succeed":
        return redirect(url_for("home_page", password_succeed=True))
    else:
        return redirect(url_for("home_page", password_fail=True))


@app.route("/register", methods=["post"])
def register_main_page():
    userid = request.form.get("reg_userid").strip()
    username = request.form.get("reg_username")
    password = request.form.get("reg_password").strip()
    proc.stdin.write("register " + userid + ' ' + password + ' ' + username + '\n')
    proc.stdin.flush()
    result = proc.stdout.readline().split()
    if result[0] == "succeed":
        return redirect(url_for("home_page"))
    else:
        return redirect(url_for("main_page", register_fail=True))


@app.route("/logout")
def logout():
    proc.stdin.write("logout\n")
    proc.stdin.flush()
    return redirect(url_for("main_page"))


if __name__ == '__main__':
    app.run(debug=True)
