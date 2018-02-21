#!/usr/bin/env python

import praw

reddit = praw.Reddit(user_agent='Apollo_Scraper',
                     client_id='UH7RWCuYA_qBIA', client_secret="nH8HZW7W8X-meUt15cZA2kFfVD8",
                     username='apollotradingbot', password='TradeCryptoBuyLambos1')

submission = reddit.submission(id=<submission_id>)
submission.comments.replace_more(limit=None)
all_comments = submission.comments.list()

