# -*- coding: utf-8 -*-

"""Test for websockets."""

from websocket import WebSocketApp
from json import dumps, loads
from pprint import pprint

URL = "wss://ws-feed.gdax.com"


def on_message(_, message):
    """Callback executed when a message comes.

    Positional argument:
    message -- The message itself (string)
    """
    pprint(loads(message))
    print


def on_open(socket):
    """Callback executed at socket opening.

    Keyword argument:
    socket -- The websocket itself
    """

    params = {
        "type": "subscribe",
        "channels": [{"name": "ticker", "product_ids": ["BTC-USD", "ETH-USD", "LTC-USD"]}]
    }
    socket.send(dumps(params))


def main():
    """Main function."""
    ws = WebSocketApp(URL, on_open=on_open, on_message=on_message)
    ws.run_forever()


if __name__ == '__main__':
    main()