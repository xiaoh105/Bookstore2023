from flask import Flask, render_template, request, redirect, url_for

app = Flask(__name__, static_folder="../../html/static", template_folder="../../html/templates")

import subprocess

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
        tmp[3].replace('|', ',')
        books.append(tmp)
    return books


@app.route("/")
def main_page(login_fail=False, register_fail=False):
    login_status = bool(request.args.get("login_fail"))
    register_status = bool(request.args.get("request_fail"))
    return render_template("main.html", books=get_books(), login_fail=login_status, register_fail=register_status)


@app.route("/login", methods=["post"])
def login_main_page():
    account = request.form.get("login_userid").strip()
    password = request.form.get("login_password").strip()
    proc.stdin.write("su " + account + ' ' + password + '\n')
    proc.stdin.flush()
    result = proc.stdout.readline().split()
    if result[0] == "succeed":
        result[1] = int(result[1])
        return render_template("login_main.html", books=get_books(), username=result[2], privilege=result[1])
    else:
        return redirect(url_for("main_page", login_fail=True))


@app.route("/register", methods=["post"])
def register_main_page():
    userid = request.form.get("reg_userid").strip()
    username = request.form.get("reg_username")
    password = request.form.get("reg_password").strip()
    proc.stdin.write("register " + userid + ' ' + password + ' ' + username + '\n')
    proc.stdin.flush()
    result = proc.stdout.readline().split()
    if result[0] == "succeed":
        result[1] = int(result[1])
        return render_template("login_main.html", books=get_books(), username=result[2], privilege=result[1])
    else:
        return redirect(url_for("main_page", register_fail=True))


@app.route("/logout")
def logout():
    proc.stdin.write("logout\n")
    proc.stdin.flush()
    return redirect(url_for("main_page"))


if __name__ == '__main__':
    app.run(debug=True)
