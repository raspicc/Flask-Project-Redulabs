from flask import Flask, render_template, url_for, request
import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

# Fetch the service account key JSON file contents
cred = credentials.Certificate('key.json')

# Initialize the app with a service account, granting admin privileges
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://labremotouni-default-rtdb.firebaseio.com/'
})


app = Flask(__name__)

@app.route('/')
def base():
    return render_template('base.html')

@app.route('/onoffled', methods=['GET', 'POST'])
def onoffled():
    if request.method == 'POST':
        selected_value = request.form.get('transporte')
        if selected_value != None:
            print(f"Valor seleccionado:{selected_value}")
            ref = db.reference('/')
            led_ref = ref.child('onoffled')
            led_ref.update({
                'STATE': int(selected_value)
            })
    return render_template('onoff_led.html')

if __name__ == '__main__':
    app.run(debug=True)
