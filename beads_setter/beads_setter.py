#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from flask import Flask, render_template, request, redirect
import os

app = Flask(__name__)

@app.route("/", methods=["GET","POST"])
def index():
    if request.method =="GET":
        posts = Post.query.all()
        return render_template('index.html')
    else:
        pub.publish(str(request.form["text"]))
        return render_template('index.html', posts=request.form.get('data'))
        
@app.route("/stop")
def stop():
    #bad approach
    os._exit(0)


if __name__ == "__main__":
    rospy.init_node('beads_map', anonymous=True)
    pub = rospy.Publisher('beads_positions', String, queue_size=100)
    r = rospy.Rate(100)
    app.run()
