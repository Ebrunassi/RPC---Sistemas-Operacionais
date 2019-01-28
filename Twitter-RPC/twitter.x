
struct tweet{
    char key;
    char usuario[20];
    char topico [20];
    char texto[80];
};

struct post_tweet_topic{
    char key;
    char topico[80];
    char usuario[20];
    float timestamp;
    char post[80];
};

struct user_follow{
    char user[40];
    char user_seguir[40];
};

struct post_users{
  char user[40];
  char topic[40];
  char post[80];
  float timestamp;
};


program CREATE_USER_PROG{
    version CREATE_USER_VERS{
        int create_user(tweet) = 5;
    }=1;
}=0x234511F5;

program FOLLOW_USER_PROG{
    version FOLLOW_USER_VERS{
        int follow_user(user_follow) = 6;
    }=1;
}=0x234511F6;

program UNFOLLOW_USER_PROG{
    version UNFOLLOW_USER_VERS{
        int unfollow_user(user_follow) = 7;
    }=1;
}=0x234511F7;

program LIST_USER_PROG{
    version LIST_USER_VERS{
        int list_user(user_follow) = 8;
    }=1;
}=0x234511F8;

program CREATE_TOPIC_PROG{
    version CREATE_TOPIC_VERS{
        int create_topic(post_tweet_topic) = 9;
    }=1;
}=0x234511F9;

program POST_TOPIC_PROG{
    version POST_TOPIC_VERS{
        post_users post_topic(post_tweet_topic) = 10;
    }=1;
}=0x234511FA;

program POST_TWEET_PROG{
    version POST_TWEET_VERS{
        post_users post_tweet(post_tweet_topic) = 11;
    }=1;
}=0x234511FB;

program LIST_TOPIC_PROG{
    version LIST_TOPIC_VERS{
        int list_topic(user_follow) = 12;
    }=1;
}=0x234511FC;

program RETRIEVE_TOPIC_PROG{
    version RETRIEVE_TOPIC_VERS{
        post_users retrieve_topic(post_tweet_topic) = 13;
    }=1;
}=0x234511FD;
