#!/usr/bin/env python3
"""
Hue Bridge Web 控制服务
"""

import json
import os
import requests
from flask import Flask, jsonify, request, abort

BRIDGE_IP = "10.1.99.114"
CONFIG_FILE = os.path.join(os.path.dirname(__file__), "hue_config.json")
BASE_URL = f"http://{BRIDGE_IP}/api"

app = Flask(__name__, static_folder="static", static_url_path="/static")


# ---------- 工具函数 ----------

def get_username():
    if not os.path.exists(CONFIG_FILE):
        abort(503, "未找到 hue_config.json，请先运行 poc.py 完成注册")
    with open(CONFIG_FILE) as f:
        cfg = json.load(f)
    username = cfg.get("username")
    if not username:
        abort(503, "hue_config.json 中缺少 username 字段")
    return username


def hue_get(path):
    try:
        resp = requests.get(f"{BASE_URL}/{get_username()}{path}", timeout=5)
        resp.raise_for_status()
        return resp.json()
    except requests.RequestException as e:
        abort(502, f"Bridge 请求失败: {e}")


def hue_put(path, body):
    try:
        resp = requests.put(f"{BASE_URL}/{get_username()}{path}", json=body, timeout=5)
        resp.raise_for_status()
        return resp.json()
    except requests.RequestException as e:
        abort(502, f"Bridge 请求失败: {e}")


# ---------- API ----------

@app.get("/api/lights")
def list_lights():
    """获取所有灯光及状态"""
    data = hue_get("/lights")
    result = {}
    for lid, info in data.items():
        result[lid] = {
            "id": lid,
            "name": info.get("name"),
            "type": info.get("type"),
            "modelid": info.get("modelid"),
            "state": info.get("state", {}),
        }
    return jsonify(result)


@app.get("/api/lights/<light_id>")
def get_light(light_id):
    """获取单个灯光详情"""
    data = hue_get(f"/lights/{light_id}")
    return jsonify(data)


@app.put("/api/lights/<light_id>/state")
def set_light_state(light_id):
    """
    控制单个灯光状态，请求体示例：
      {"on": true}
      {"on": true, "bri": 200}
      {"on": true, "bri": 150, "hue": 46920, "sat": 230}
      {"on": true, "ct": 370}
    """
    body = request.get_json(silent=True) or {}
    allowed = {"on", "bri", "hue", "sat", "ct", "xy", "alert", "effect", "transitiontime"}
    state = {k: v for k, v in body.items() if k in allowed}
    if not state:
        abort(400, "请求体为空或不含有效字段")
    result = hue_put(f"/lights/{light_id}/state", state)
    return jsonify(result)


@app.get("/api/groups")
def list_groups():
    """获取所有灯组"""
    data = hue_get("/groups")
    return jsonify(data)


@app.put("/api/groups/<group_id>/action")
def set_group_action(group_id):
    """控制整个灯组"""
    body = request.get_json(silent=True) or {}
    allowed = {"on", "bri", "hue", "sat", "ct", "xy", "alert", "effect", "transitiontime", "scene"}
    action = {k: v for k, v in body.items() if k in allowed}
    if not action:
        abort(400, "请求体为空或不含有效字段")
    result = hue_put(f"/groups/{group_id}/action", action)
    return jsonify(result)


@app.get("/api/scenes")
def list_scenes():
    """获取所有场景"""
    data = hue_get("/scenes")
    return jsonify(data)


# ---------- 前端页面 ----------

@app.get("/")
def index():
    return app.send_static_file("index.html")


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
