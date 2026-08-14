A mod by human for human, I do not like the usage of generative AIs. Also, we stand for Trans people right :3

# Twitch consumer demo

This mod is to show what can be done via the [Twitch loader](https://github.com/Noaseto/TwitchLoader) service

Its goal is to stay the minimal code needed to interact with it so people can plug this service easily

# What can be done

After enabling both this mod and the Twitch loader, load a savefile and go to a place you like.

| What to do             | What will happen              |
|------------------------|-------------------------------|
| Type a message in chat | it will spawn a rupee         |
| Get a follower         | it will spawn a heart         |
| Get a subscriber       | it will spawn a blue rupee    |
| Type "rupee"           | it will spawn 20 green rupees |

I do not stream regularly so I do not have channel points nor subs, and can't verify that it works (I think I could use
twitch CLI, but I didn't bother yet). It should be as easy as that, enjoy :>

PS: I have no clue why but in some areas the createItem command crashes the game, it works in ordon village though.

# Where to find it

In the mod.cpp, the mod_update will call the service to retrieve all events since last frame and then call the
handle_twitch_event method which is simply a switch case based on the TwitchEventType enum. For more info, look
at the TwitchLoader sequence diagram.
