/*	SCCS Id: @(#)engrave.c	3.4	2001/11/04	*/
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "lev.h"
#include <ctype.h>

STATIC_VAR NEARDATA struct engr *head_engr;

#ifdef OVLB
/* random engravings */
static const char *random_mesg[] = {
	"Elbereth",
	/* trap engravings */
	"Vlad was here", "ad aerarium",
	/* take-offs and other famous engravings */
	"Owlbreath", "Galadriel",
	"Kilroy was here",
	"A.S. ->", "<- A.S.", /* Journey to the Center of the Earth */
	"You won't get it up the steps", /* Adventure */
	"Lasciate ogni speranza o voi ch'entrate.", /* Inferno */
	"Well Come", /* Prisoner */
	"We apologize for the inconvenience.", /* So Long... */
	"See you next Wednesday", /* Thriller */
	"notary sojak", /* Smokey Stover */
	"For a good time call 8?7-5309",
	"Please don't feed the animals.", /* Various zoos around the world */
	"Madam, in Eden, I'm Adam.", /* A palindrome */
	"Two thumbs up!", /* Siskel & Ebert */
	"Hello, World!", /* The First C Program */
	"You've got mail!", /* AOL */
	"May the force be with you!",
	"As if!", /* Clueless */
	/* [Tom] added these */
	"Y?v?l s??ks!", /* just kidding... */
	"T?m ?as h?r?",
	/* Tsanth added these */
	"Gazortenplatz", /* Tribute to David Fizz */
	"John 3:16", /* You see this everywhere; why not here? */
	"Exhale! Exhale! Exhale!", /* Prodigy */
	"All you need is love.", /* The Beatles */
	"....TCELES B HSUP   A magic spell?", /* Final Fantasy I (US) */
	"Madam, in Eden, I'm Adam.", /* A palindrome */
	"Turn around.", /* Various people at various times in history */
	"UY WUZ HERE", /* :] */
	"Time flies when you're having fun.", /* Who said this first, anyway? */
	"As if!", /* Clueless */
	"How bizarre, how bizarre.", /* OMC */
	"Silly rabbit, Trix are for kids!", /* Trix */
 	"I'll be back!", /* Terminator */
 	"That is not dead which can eternal lie.", /* HPL */
"we serve perfect meals and only exdule all cavepeople- The Chromatic Dragon",
"we serve perfect meals and only exdule all cavepeople- The Chromatic Dragon",
"we serve perfect meals and only exdule all cavepeople- The Chromatic Dragon cafe",
"we serve perfect meals and only exdule all cavepeople- The Chromatic Dragon cafe",
"**FR33 P0T10N 0F FULL H34L1NG**",
"An HTML attachment was scrubbed...",
"Are YOU tired of the same old messages? Go to http://alt.org/nethack/addmsgs/ now!",
"Badger badger badger badger badger badger MUSHROOM MUSHROOM!",
"CONGRATULATIONS! you are now the proud owner of 40 newts.",
"Christmas Greetings!",
"Crush the enemy, see them driven before you, and hear the lamentations of their women!",
"Dear $playername, how do you spell Elbereth? Singed- A friend",
"Dear $playername, your $fruit is in another castle.",
"Dear Dudley, what would you say are your greatest weaknesses?",
"Do you want to change that garter snake into a raging python?",
"Do you want to start? Take the 5:45 Shibuya bound train from Jiyuugaoka Station",
"Engraving Moloch works as engraving Elbereth. Just see how that a turns to flee from here!",
"Find a small token to represent your Owlbear",
"HOT inside stock tip - Yendor Industries (YNDR) STRONG BUY!",
"Has anyone really been far even as decided to use even go want to do look more like?",
"Have you heard of a spell called Explosive Runes?",
"Hello.",
"How are you gentlemen?",
"Spam Hummmmmmmmmmmmmmmmmmmmmmm...",
"I am Nigerian prince HORRE YED MOBUTU contacting you regarding the transfer of $10000000 zorkmids",
"I've had it with you $playername !",
"IM in ur base, killing ur doods!!!111",
"LOL ROFL LMAO",
"Lolol!",
"MAKE ZORKMIDS FAST, GUARANTEED!",
"Made you look!",
"Magic-B-Here sells wands of not exactly finest quality! Bye today!",
"Make a contract with me to become a puella magi!",
"Me, too!",
"PENIS PENIS PENIS PENIS PENIS PENIS PENIS PENIS PENIS PENIS PENIS PENIS PENIS PENIS PENIS PENIS",
"Play FREE roguelike game - http://crawl.akrasiac.org/",
"Please return to sender (Asmodeus).",
"Please send 15300 zorkmids security deposit to help me move 11 million (Z) out of Ludios!",
"Pole Arm Wax! Only 350 Zk.",
"Potion of generic C1al1s, only 10 zorkmids, satisfaction guaranteed!",
"Red in the morning, sailor's warning.",
"SEVEN DAYS",
"Sale @ Izchak's! The first five visitors will get a free lamp!",
"Scroll of punishment. Apply directly to the forehead.",
"Send $5 to the return address, and get a free subscription to Thumbs magazine!",
"Slash'EM Free Forever, Come Play My Lord *INSERT SUCCUBUS PIC HERE*",
"Sword enlargement scrolls! Base price: 60 zorkmids!",
"System shut down in 1 minute(s)",
"TYPE 111 FOR HOT SUCCUBUS LOVIN'",
"Thanx Maud",
"The WOW starts now - http://www.microsoft.com/windowsvista/",
"The best escort services! Meet the most intelligent succubi and incubi! Call 1-800-HOTDATE!",
"This is Spam Enjoy!",
"This mail complies with the Yendorian Anti-Spam Act (YASA).",
"This message has been bounced. If you recieved this successfully, ignore this.",
"This scroll was made with cockatrice leather. Have a nice day.",
"This space intentionally left blank.",
"Thou needst no longer toil with a mere quarterstaff!",
"Try new magic Scroll! Up to three inches GUARANTEED!",
"Undeliverable mail returned to sender: no such address <rodney@dungeons.doom>",
"WAN7 +0 EN L4GE Y UR PEN???",
"Watch out! The-Gnome-With-The-Wand-Of-Death is behind that door!",
"While your character doesn't need socialising, remember that you do. Don't be a tool.",
"YENDOR EATS CHEEZE WIZZ",
"Yo Momma!",
"You are about to die... so why not get a cheap LIFE INSURANCE?",
"You are the 100 millionth person to read this message! Wait 500 turns for your FREE Iron Chain.",
"You got mail. From AOL!",
"You may already have won $1000!",
"You're doing great! Keep up the good work!",
"You've been pre-approved for the Platinum Yendorian Express card!",
"Your Dad On A Sybian",
"Your porn stash has been located. Have a good day.",
"g3t bl3ss3d skrol1z of 3nch4n7 w34pon ch33p! add inchez to your Sword!",
"http://www.tvtropes.org/",
"qwertyuiopasdfghjklzxcvbnm",
"wow. such meme. so epic. much random.",
"Engrave a wise saying and your name will live forever. -- Anonymous",
"You smite the vampire. It explodes! You seem unaffected by it.",
"dilbert aint gay - tupac",
"$playername sux!",
"+7 Magicbane. Do you want to kill monsters, or do you want to fool around?",
"--More--",
"/join #nethack",
"1/0",
"1337",
"2, 12, 1112, 3112, 132112, 1113122112, ...",
"3ngr@Vr VVu$ }{3@r 3|-|d }{e 1eft $u/V-133tsp3@k!!!!1111!!!oneoneone",
"4, 8, 15, 16, 23, 42",
"666 420 Tongue Kiss Your Father",
"<-- Go that way!",
">:D",
"A - cursed loadstone.",
"A horse is a horse of course...",
"Aaoooooooooooo! Werewolves of Yendor!",
"All work and no play makes Jack a dull boy",
"Allburrith",
"Always Be Closing!",
"At last... the amulet ^n my grasp..... hidden in the sec+et room on th! %3rd level...",
"Bark!",
"Beware the exploding cow!",
"Blessed Greased +5 silver dragon scale mail buried here",
"CAPS LOCK IS CRUISE CONTROL FOR COOL",
"Call Before You Dig",
"Cancel this engraving",
"Chainsaw was here! RAAAAAAAGH!",
"Clothed for inventory",
"Come on, $playername, don't dawdle! You have almost missed the train!",
"Computers: Can't live with 'em, can't play Nethack without 'em.",
"Cool story, bro.",
"Destroy the amulet on this level.",
"Did you know the witch-woman jenka had a brother?",
"Dig Here for a +5 pair of Speed Boots!",
"Dig down!",
"Dig for Victory here",
"Dig here --- X",
"Dig here for Iraq's WMDs",
"Dig here.",
"Do not feed the pink elephant",
"Do not move forward",
"Do not taunt happy fun cockatrice",
"Don't Panic",
"Don't go that way.",
"Don't look behind you.",
"Don't worry. You're still illiterate.",
"Duck!",
"E=mc ... no, that's not it.",
"ERAU QSSI DRLO WEHT",
"ETAOIN SHRDLU",
"Eat at Joe's",
"Elbcreth",
"Elber",
"Elberek? Albereth? Albert?",
"Elberet",
"Elbereth",
"Elbereth",
"Elbereth was engraved here, but it's no longer engraved here. You're too late, loser!",
"Elberith Elbireth Elberth Lbereth Elbreth",
"Elebird",
"Eleburff",
"Elf Breath",
"Engraving with a hammer is hard!",
"Environmentally sensitive area -- no farming",
"Even in your NetHack game, F.O.E.!",
"Find the computer room!",
"First!",
"For a good time call 867-5309",
"For a good time, call 1-900-FOOCUBI!",
"Friendlydads.net friendliest dads on the Net!",
"Gaius Julius Primigenius was here. Why are you late?",
"Galadriel",
"Gandalf for president!",
"Gatorade. Is it in you?",
"George plonks ferrets.",
"Get A BRAIN! MORANS",
"Gilthoniel",
"Go Left -->",
"Go around! Go around and around and around! What fun! I'm so happy!",
"Go see Snakes on a Plane.",
"Got milk?",
"Haermund Hardaxe carved these runes",
"Hastur Hastur H%s+ur",
"He who is valiant and pure of spirit may find the Holy Grail in the Castle of Arrrgh",
"HeadOn! Apply directly to the forehead! HeadOn! Apply directly to the forehead!",
"Hello world!",
"Here be dragons.",
"Here lies Bob; he tried an acid blob",
"Here lies all hopes of a cheedar cheese pie.",
"Here there be soldier ants.",
"Hey, you! Stupid-looking kid! Look down here!",
"Hoc est qui sumus.",
"How much wood could a w**dc***k chuck if a w**dc****k could chuck wood?",
"Htereble",
"I AM ERROR.",
"I am a fish.",
"I bet you're wishing that you had the ability to press ctrl + w",
"I couldn't afford a stone!",
"I love you Stacy, please marry me! AArrghh!",
"I prepared Exploding Runes this morning.",
"I prepared Explosive Runes this morning.",
"I warned you about stairs bro!",
"I wasn't here!",
"I wasn't kidding when I said pay! Now, you'll pay the ultimate price!!",
"I went that way! -->",
"I wonder what's for dinner...",
"I'm George W. Bush, and I approve this message.",
"I'm sorry, but the Amulet of Yendor is in another plane!",
"IT HIURTS",
"Ia! Ia! Cthulhu Fhtagn!",
"If you can read this, you're standing too close",
"If you find silver dragon scale mail on this spot, consider yourself lucky.",
"In Soviet Russia, the amulet ascends with you!",
"It explodes!",
"It hits! It hits! You regret eating that one more quesadilla! You die... --More--",
"It is dark. You may be eaten by a grue.",
"It's a trick. Get an axe.",
"Jesus was here 51 BC",
"Joseph of Arimathea was here!",
"June 5, 2009: I've been wandering these dungeons for days... I want to see my $playername again",
"Keep off the grass!",
"Kilroy was here.",
"Klaatu barada nikto!",
"Klein bottle for sale -- enquire within.",
"Look behind you, a three-headed monkey!",
"Look! Over there!",
"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut...",
"Madam, I'm Adam!",
"Marvin was here.",
"Maxim 1: Pillage, Then Burn",
"Need a light? Come visit the Minetown branch of Izchak's Lighting Store!",
"Need more time? Pray for the Time Goddess! **REQUIRES MONEY**",
"No! Not another new-",
"Nodnol 871 Selim",
"Not Elbereth",
"OH SH-",
"OW! top TREADING ON ME!",
"On this floor is engraved image of young $playername. The $playername is in fetal position.",
"On this floor is masterfully engraved image of : and @. The newt bites the hero. The hero dies...",
"Oo ee oo ah ah ting tang walla walla bing bang",
"Open for inventory",
"Owl Breath",
"Owlbreath",
"Pay Attention!",
"Perception changes with perspective",
"Please insert disk 2 to continue.",
"Poot dispenser here",
"Preeeeeeeeeeeeeow.",
"REDRUM",
"Rainbow Dash is the best pony.",
"Real men play female characters.",
"Rodney was here",
"Rosebud.",
"Run for it! It's a rigged bones file!",
"Save Ferris!",
"Save now, and do your homework!",
"See the TURTLE of enormous girth! On his shell he holds the earth.",
"Snakes on the Astral Plane - Soon in a dungeon near you",
"Sometimes I feel that people can read me like an engraving...",
"Sorry, $playername, but The Amulet of Yendor is on another dungeon level!",
"Stuff to remember: don't kick sinks.",
"The Gnome-with-a-Wand-of-Death was here.",
"The Vibrating Square",
"The Vibrating Square is NOT here",
"The forgotten pet $dog has come! An overpowered vicious dog. Beware it's deadly bite!",
"The grass is always greener on the other side of your sunglasses.",
"The moon rules - #1",
"The preparations are complete. There is no hope for $playername now.",
"The treasure is buried uuddlrlrba from here.",
"There is nothing engraved here. Really.",
"There was a hole here. It's gone now.",
"They say god saved the queen",
"They say that They can never be trusted.",
"They say that the Spider Caves have a wand of death... Oh wait... That was SLASH'EM, wrong game",
"They say that there's a stalker behind you",
"They say that you are what you eat.",
"They say that you can't trust what they say.",
"This engraving is totally unextrordinary. Wait! No! It says - no, it's unextrordinary.",
"This is a pit!",
"This is a pit!",
"This is not the dungeon you are looking for.",
"This message is false.",
"This space intentionally left blank.",
"This square deliberately left blank.",
"This statement is false.",
"This tile tells you that you need at least AC -15 to clear the castle.",
"This way for egress.",
"Toy Story 2 was OK.",
"Try ?? , : :: /f !, You",
"Twilight Sparkle has bacon hair.",
"Unicorn skin tastes likes Jesus.",
"Vlad wasn't here",
"W.A.S.T.E.",
"WARNING! Do not read this message.",
"WARNING: CONSTRUCTION AHEAD.",
"WOT, NO GYPSIES?",
"WTF",
"Warning, Exploding runes!",
"Watch out! The-Gnome-With-The-Wand-Of-Death is behind that door!",
"We are not amused.",
"Where are the ats of yesteryear?",
"Who is this foreign host and why does he keep closing my sessions?",
"Why are you looking down here?",
"Why are you reading this?",
"Why's everybody always picking on me?",
"X marks the spot",
"X marks the spot.",
"Yeah, well, y'know, that's just like, your opinion, man.",
"You are already dead!",
"You are here.",
"You are here.",
"You aren't going to ascend by staring at your shoes.",
"You can't help being ugly, but you could stay home.",
"You die...",
"You die...",
"You fall into a bottomless pit. You die...",
"You step on a polymorph trap!",
"Your Dad On A Sybian",
"http://www.tvtropes.org/",
"lol",
"mde.tv",
"mexican jumping beans",
"rerecord don't fade away",
"romanes eunt domus",
"romanes eunt domus",
"shawnz sux.",
"you are the one millionth visitor on this place! please wait 200 turns for a wand of wishing.",
	/* From NAO */
	"Arooo!  Werewolves of Yendor!", /* gang tag */
	"Dig for Victory here", /* pun, duh */
	"Don't go this way",
	"Gaius Julius Primigenius was here.  Why are you late?", /* pompeii */
	"Go left --->", "<--- Go right",
	"Haermund Hardaxe carved these runes", /* viking graffiti */
	"Here be dragons",
	"Need a light?  Come visit the Minetown branch of Izchak's Lighting Store!",
	"Snakes on the Astral Plane - Soon in a dungeon near you!",
	"There was a hole here.  It's gone now.",
	"The Vibrating Square",
	"This is a pit!",
	"Warning, Exploding runes!",
	"Watch out, there's a gnome with a wand of death behind that door!",
	"X marks the spot",
	"X <--- You are here.",
	"You are the one millionth visitor to this place!  Please wait 200 turns for your wand of wishing.",

	/* http://www.alt.org/nethack/addmsgs/viewmsgs.php */
	"Badger badger badger badger badger badger MUSHROOM MUSHROOM!",
	"Dig here for a +5 pair of Speed Boots!",
	"If you find speed boots on this spot, consider yourself lucky.",
	"Gandalf for president!",
	"Fnord",
	"\"Engrave a wise saying and your name will live forever.\" -- Anonymous",
	"In Soviet Russia, the amulet ascends with you!",
	"Need a light? Come visit the Minetown branch of Izchak's Lighting Store!",
	"The Vibrating Square is NOT here",

	/* From UnNetHack */
	"She's watching you!", /* Portal */
	"The cake is a lie!", /* Portal */
	"cjohnson/tier3", /* Portal */
	"What's the sitch?", /* Kim Possible */
	"So not the drama.", /* Kim Possible */
	"Sudden decompression sucks!", /* Space Quest 3 death message */
	"Thanks for playing UnNetHackPlus.  As usual, you've been a real pantload!", /* Space Quest 4 death message, paraphrased */
	"Colorless green ideas sleep furiously.", /* Noam Chomsky */
	"Time flies like an arrow.", /* linguistic humor */
	"All Your Base Are Belong To Us!", /* Zero Wing */
	"Numa Numa", /* Internet Meme */
	"I want to believe!", /* X-Files */
	"Trust No One", /* X-Files */
	"The Truth is Out There", /* X-Files */
	"Look behind you, a Three-Headed Monkey!", /* Monkey Island */
	"Ifnkovhgroghprm", /* Kings Quest I */
	"Cookies are delicious delicacies", /* Mozilla Firefox */

	/* From UnNetHackPlus */
	"Losing is fun!", /* Dwarf Fortress */
	/* Murphy's laws */
	"Cheer up, the worst is yet to come...",
	"Computers let you waste time efficiently",
	"If something cannot go wrong at all, it will go wrong in a very spectacular way.",
	"If everything seems to be going well, you have obviously overlooked something",
	"If anything can go wrong, it will",
	"When things go from bad to worse, the cycle repeats.",
	"Being dead right, won't make you any less dead.",
	"If all else fails, hit it with a big hammer.",
	"If many things can go wrong, they will all go wrong at the same time.",
	/* end of Murphy's laws */
	"That is not dead which can eternal lie, and with strange aeons even death may die.", /* H. P. Lovecraft */
	"Ph'nglui mglw'nafh Cthulhu R'lyeh wgah'nagl fhtagn.", /* H. P. Lovecraft */
	"Conducts are about player skill, not about obsessive-compulsive disorder", /* TJR on RGRN */
	"That is not dead which can eternal lie, and with strange aeons even vanilla might get released.", /* bhaak on http://nhqdb.alt.org/?301 */
	/* http://donjon.bin.sh/fantasy/random/#dungeon_graffiti */
	"When song becomes silence, the Wand of Aether shall be destroyed",
	"Upon the Night of Dweomers, when iron is made flesh and the Golden Scepter is wreathed in flames, the Copper Tower shall be restored",
	"Run away!",
	"<--",
	"-->",
"This door leads to the women's locker room. Use a wand of make invisible on yourself before entering.",
"You stand at the entrance to XXX SEX SHOP!!!",
"Draw it! Draw it!",
"Pong begin broadcasting!",
"TEAM JACOB MOTHERFUCKERS. Gotta banish that douchebag Edward to Gehennom for all eternity.",
"The effect of spray disappears.",
"There is the tree with fruit!",
"The sound is loud, water aboves fall!",
"The door is locked. Can't be opened.",
"You win? Very good. Now I believe the girls is good too.",
"You ride a bike to advertise?",
"A bright bike!",
"Very lovrly!",
"My son collect too, understand the monster, Zhenqian says so. The aid center of monster just called me.",
"It's by my brother just now I wanted to tell you his tel number!",
"The left passages do not have the ticket, how about them?",
"Troubled you? Xiaojin comes welcome to take again!",
"It is nothing the olden-tortoise water-pot is belonded to you.",
"This time you will take it to the center of monster to be treated.",
"Welcome! This is Xiaojin Store!",
"There are sorts of things no if the game shop in this city don't go out to exchange.",
"Ground floor is service counter for free.",
"The second is a market store.",
"The fourth is a drugstore.",
"The fifth is the skill machine counter.",
"The sixth is a square.",
"The garret is watching tower.",
"You can register ten with pocket gearing.",
"Do you turn to the 'lucky channel' programme?",
"You are very good!",
"The coins increasing,and laughing endless!",
"Welcome! Play the game, put the coin into there, exchange the goods!",
"Machine is interesting compared with worm machine.",
"This is a life games rotate quickly.",
"Card machine, you can imagine easily!",
"The cup to drink is there, it has a sweet taste!",
"Welcome to the trade center here, the people who are stranger each other, exchange monaster casually.",
"Does the monaster owned each other to be exchange now?",
"It needs times to find the exchanged object, come after a while!",
"You sit on the cockatrice corpse. It is not very comfortable.",
"The mystic egg is prepared for you specially send it at once there are many rooms in the center.",
"Make preparations now wait for a moment please!",
"Before finding the news wrote a report please!",
"Do you know the news?",
"The data of the news is wrong - see you next!",
"Then the monster you want will let her send it out. If everybody do so, we can communicate with various of people.",
"It's a female cat called Anxiong whose name is as same as father's.",
"The palace is writed how to use the radio.",
"We will realize the wish of the coachs all over the world. Let's work hard!",
"Do you pay homage to the spirit of the forest? The forest spirit surpasses space and time to protect the forest.",
"She is the grass monster originally.",
"It looks like the picture on the relic wall, if it is like as a monster, according to the reason there are many.",
"The front.... En?What? You use the secret sign!",
"I like to think of the question about the relic.",
"This relic built 1500 years ago! But,who had built it.",
"The libyan relic!",
"The graduate school of libyan relic the question about relic!",
"It is made patterning foregone monsters.",
"This is the appearance drawn on the wall!",
"There is a big hole on the wall!",
"Welcome to relic small house! There are the appearance of monsters drawn remote time ago. Move it to look please!",
"There is nothing just before you come there to look at the wall!",
"Under your help to our investigation about the relic, we have a father understanding.",
"The thing made patterning foregone monsters.",
"A big room but nothing!",
"A holy appearance, it is a mystic place!",
"Good! The alteration for monsters badge is achieved! Supplement a unknown badge.",
"Under your help to investigate we have known the relic, as if the relic was built for the monsters to inhabit!",
"This is the first superintendent for libya graduate school the photo of Namuluoji Dr!",
"Many profound books. 'Archaian relic' 'Riddle of archaian civilization'",
"That is the soul of coach!",
"It is yedong's tail to be cut off! There are some mails,do you want to read?",
/* contributed by members of NetHackWiki */
	"Please disable the following security protocols: 2-18, 1-12, 1-0", /* the Submachine series */
		"Our coil is broken, and we don't have a replacement. It's useless. It's so useless.", 
	
/* contributed by members of the Nethack Usenet group, rec.games.roguelike.nethack */
	"Which way did he go, George, which way did he go?",
	"Until you stalk and overrun, you can't devour anyone.",
	"Fee.  Fie.  Foe.  Foo.",
	"Violence is the last refuge of the incompetent.",
	"Be vewy vewy quiet.  I'm hunting wabbits.",
	"Eh, what's up, doc?",
	"We prefer your extinction to the loss of our job.",
	"You're weird, sir.",
	"I do not know for what reason the enemy is pursuing you, but I perceive that he is, strange indeed though that seems to me.",
	"When it is falling on your head, then you are knowing it is a rock.",
	"The very best mail is made from corbomite.",
	"Woah, duude, look at all the colors.",
	"I'm bringing home a baby bumblebee.",
	"Now, if you'll permit me, I'll continue my character assassination unimpeded.",
	"Cry havoc, and let slip the dogs of war.",
	"Virtue needs some cheaper thrills.",
	"I just saw Absolom hanging in an oak tree!",
	"And I may do anything I wish as long as I squeeze?",
	"Marsey Dotesin Dosey Dotesin Diddley Damsey Divey.",
	"I knew I should have taken that left turn at Albuquerque.",
	"YOU BROKE MY SWORD!",
	"An atom-blaster is a good weapon, but it can point both ways.",
	"You may have already won ten million zorkmids.",
	"They say the satisfaction of teaching makes up for the lousy pay.",
	"Tharr she blows!",
	"Perhaps today is a good day to die.",
	"Sufferin' succotash.",
	"To succeed, planning alone is insufficient. One must improvise as well.",
	"I love it when a plan comes together.",
	"Very fond of rumpots, crackpots, and how are you Mr. Wilson?",
	"No matter how valuable it might be, any man's life is still worth more than any animal's.",
	"No clock for this.  Flyin' now.",
	"Of course, you realize, this means war.",
	"She's fast enough for you, old man. What's the cargo?",
	"There're few who haven't heard the name of Gurney Halleck.",
	"Do you like mayonnaise and corn on your pizza?",
	"I am that merry wanderer of the night.",
	"Don't try any games.  Remember I can see your face if you can't see mine.",
	"Ah hates rabbits.",
	"Captain, there be whales here!",
	"Mimesis is a plant.  Go Bucks!",
	"That's all we live for, isn't it?  For pleasure, for titillation?",
	"The only brew for the brave and true comes from the Green Dragon.",
	"There is an old Vulcan proverb: only Nixon could go to China.",
	"Yo, put me in a trebuchet and launch me to the stratosphere.",
	"Lorem ipsum dolor sit amet, consectetur adipisici elit, sed eiusmod tempor incidunt ut labore et dolore magna aliqua.",
	"Sola lingua bona lingua mortua est.",
	"Mairzy Doats And Dozy Doats And Liddle Lamzy Divey A kiddley divey too, wouldn't you?",
	"Madam, I may be drunk, but you are ugly, and in the morning I shall be sober.",
	"Outside of a dog, a book is a man's best friend.",
	"Inside of a dog, it's too dark to read." ,
	"You are superior in only one aspect. You are better at dying.",
	"No McFly ever amounted to anything in the history of Hill Valley!",
	"You've got to ask yourself one question: 'Do I feel lucky?' Well, do ya, punk?",
	"In this pearl it is hard to see a Strange Loop.",
	"That is because the Strange Loop is buried in the oyster -- the proof.",
	"I love the smell of napalm in the morning.",
	"Et tu, Brute?",
	"The conditions were difficult. Several contestants were maimed, but... I was triumphant. I won Champion Standing.",
	"We're not gonna take it. No, we ain't gonna take it. We're not gonna take it anymore.",
	"Absurd!  Your 'MU' is as silly as a cow's moo.",
	"Hasta la vista, baby.",
	"I see...  I see...  I see a lavender man...  bearing tidings.",
	"It is a sign of creative genius to reconcile the seemingly disparate.",
	"It pays to be obvious, especially if you have a reputation for subtlety.",
	"Bother. I've left my new torch in Narnia",
	"If you put into the machine wrong figures, will the right answers come out?",
	"I am not able rightly to apprehend the kind of confusion of ideas that could provoke such a question.",
	"Colorless green ideas sleep furiously",
	"We fear a few dwarf watercraft stewardesses were regarded as desegregated after a great Texas tweezer war.",
	"I'll kill him in Honolulu, you unhook my pink kimono.",
	"The clown can stay, but the Ferengi in the gorilla suit has got to go.",
	"All roads lead to Trantor, and that is where all stars end.",
	"It is a far, far better thing I do than I have ever done. It is a far, far better rest I go to than I have ever known.",
	"Yes, this is Network 23. The net-net-net-net-network that puts it right... where they want you to be!",
	"You're listening to the E.I.B Radio Network.",
	"We shall this day light such a candle, by God's grace, in England, as I trust shall never be put out.",
	"How about a little fire, scarecrow?",
	"And if you're the kind of person who parties with a bathtub full of pasta, I suspect you don't care much about cholesterol anyway.",
	"I AM A SEXY SHOELESS GOD OF WAR!!!",
	
	"Bad Wolf", /* Dr. Who */
	"It's only the end if you want it to be.", /*Batgirl*/
	"Mistakes were made.",
	"But at what cost?",
	"For we wrestle not against flesh and blood, but against principalities and powers.", /* Ephesians 6:12 */
	"Life is truth, and never a dream...", /* Persona 4 */
	"fire, walk with me.", /*Twin Peaks*/
	"redefine happiness", /* Blue Oyster Cult */
	"no more sadness, no more anger, no more envy...", /*Halo*/
	"Even false things are true",	"FIVE TONS OF FLAX!",	"Fnord", /*Discordianism*/
	"Ash nazg durbatuluk, ash nazg gimbatul, ash nazg thrakatuluk, agh burzum-ishi krimpatul", /*the Lord of the Rings*/
	"No Fate", /*Terminator*/
	"This world can have but one Emperor.", /*FF II*/
	"We shall devour your light, and use it to return this world to the Void!", /*FFIII*/
	"My hatred will not be stanched until it has consumed all else!", /*FFV*/
	"Life... dreams... hope... Where do they come from? And where do they go...?", /*FFVI*/
	"I will NEVER be a memory.", /*FFVII Advent Children*/
	"All the world will be your enemy, Prince With A Thousand Enemies...", /*Watership Down*/
		"...and when they catch you they will kill you... but first they must catch you.",
	"This statement is false.", /*Logical contradiction*/
	"Quod Erat Demonstrandum",	"anno Domini",	"per centum", /* QED, AD, percent */
	"I was so surprised, I CLASSIFIED INFORMATION, I really did!", /*Haruhi Suzumiya*/
	"[REDACTED]",	"[DATA EXPUNGED]",	"[DATA PLUNGED]",	"[DATA EXPANDED]",	"I am a toaster!", /* SCP Foundation */
	"I prepared Explosive Runes this morning.", /*Order of the Stick*/
		"In any battle, there's always a level of force against which no tactics can succeed.",
	"1002: He is Not that Which He Says He Is.", /*Good Omens*/
		"3001: Behinde the Eagle's Neste a grate Ash hath fellen.",
	"VI. You Shall Not Subject Your God To Market Forces!",	/*Discworld*/
		"Nae quin! Nae laird! Nae master! We willna be fooled again!",
		"Morituri Nolumus Mori",
		"End-of-the-World Switch. PLEASE DO NOT TOUCH",
		"Give a man a fire and he's warm for a day, but set fire to him and he's warm for the rest of his life.",
		"Do not, under any circumstances, open this door",
		"WORDS IN THE HEART CANNOT BE TAKEN",
		"Slab: Jus' say \"AarrghaarrghpleeassennononoUGH\"",
		"Ahahahahaha! Ahahahaha! Aahahaha!\nBEWARE!!!!!\nYrs sincerely\nThe Opera Ghost",
		"Bee There Orr Bee A Rectangular Thyng",
	"NOT A Secret Base", /*Gunnerkrigg Court*/
	"Short to long term memory impaired. Go to Robotics Building. Explain about Bowman's architecture. Write down everything.", /*Freefall*/
	"I SAW THE EYE. Over the horizon, like a rising sun!", /* Dresden Codak */
		"That's Science, isn't it? The perpetual horizon.\nBut we found a way, haha, we found a way around that!",
		"What would YOU give up to... to see it through?",
	"Don't trust the skull.", /* Cynarfpncr: Gbezrag (mild spoiler) */
	"We do not threaten or warn or advise. We tell the truth.", /* The Mansion of E */
		"What you stole is even more dangerous and precious than you know.",
		"In the end, you will fly away and live forever, and you will leave the world shattered in your wake.",
		"You will return to where you started, and you will become what you hate.",
		"And in doing so, you will restore the world to what it was.",
	"Fall in a more hardboiled manner.", /* MS Paint Adventures */
		"Since you are reading this, chances are you have already installed this game on your computer.\nIf this is true, you have just participated in bringing about the end of the world.\nBut don't beat yourself up about it.",
		"YOU HATE TIME TRAVEL YOU HATE TIME TRAVEL YOU HATE....",
		"It begins to dawn on you that everything you just did may have been a colossal waste of time.",
	"We will BUILD heroes!", /* THE PROTOMEN */
		"If you replace the working parts, you get a different machine.",
		"If you destroy the working parts, what you get is a broken machine.",
	"Never tell me the odds!" /* Starwars */,
	"Needle-like the new log to know that SlashEM Extended are you ...",
	"http://nethackwiki.com/wiki/Slash%27EM_Extended I will not cease to poison by carbon monoxide",
	"after opportunity to kill Cthulhu Geenome lightsaber, playing for the Khajiit-transsexual talk about that in other games is the freedom to choose make me laugh only",
	"Yes, we'll see a list of races and added roles such trash",
	"This area is property of Team Dislexiapater.",
	"Anna's Hussy Club - Main Entrance",
	"Marking left by Arabella to denote space for nasty traps",
	"ElberethElbereth", "ElberethElberethElbereth",
	"ElberetnElherethLlberethElbcrethElbere?hElbercthFlbereth", "Ylbereth",
	"I never hit with a wielded weapon, but I changed form six times.",
	"Always name your kitten after a famous wizard.",
	"You are in a maze of twisty little passages, all alike.",
	"You have died of dysentery.",
	"The cake is a lie.",
	"The cake is a lie, says Super Mario after rescuing the princess 5 minutes into the game in Super Mario 64 by using TAS tools",
	"We all make choices, but in the end our choices make us.",
	"We juggle priceless eggs in variable gravity.", /* Gateway */
	"They therefore went up here with much agility and speed", /* Borders of Infinity (and originally from Pilgrim's Progress) */
	"We are the dead.", /* 1984 */
	"To sleep, perchance to dream, aye, there's the rub.", /* Hamlet */
	"When the goat turns red strikes true.", /* Order of the Stick */
	"It is a far, far better thing I do than I have ever done.", /* A Tale of Two Cities */
	"All dungeon adventurers are equal; but some are more equal than others.", /* Animal Farm */

	"Dare mighty things.", /* Moto of the Mars Science Laboratory */
	"are we cool yet?", /* SCP Foundation */
	"I seem to be having tremendous difficulty with my lifestyle", /* Hitchhiker's Guide to the Galaxy */
	"The flow of time itself is convoluted, with heroes centuries old phasing in and out.", /* Dark Souls */

	"Who are you?", "What do you want?", "Why are you here?", "Where are you going?", /*Babylon 5*/
	"Beware of the weeping angels", /* Doctor Who */
	"Duck, Sally Sparrow", /* Doctor Who */
	"Standing on this square increases your weeping angel contamination by 50 per turn",
	"You made a mistake by stepping on this square - now dnethack's elder priest was spawned on the current level.",

	"4 8 15 16 23 42", /* Lost */
	
	"This too shall pass.", /* medieval Persian adage */

	"Write down the coordinates, he said.", /* the Submachine series */
	"...look for a green leaf...",
	"...bring it to the statue...",
	"...you should arrive at the lab...",
	"...or somewhere nearby...", 

	"Here would be graffiti about one of the first nine ascenders of dnethack, but this isn't dnethack.",
	"AmyBSOD was here. AmyBSOD followed AmyBSOD. (repeat 6 times) Why is no one else ascending SLEX?",
	"Why don't you put some effort into it and ascend? SLEX's demigod bar is waiting for you!",

	"@ <--- You are here.",
	
	"Jesus was here 2/15/57 B.C.", /* Family Guy */
	"If you have time to read this, you should get to the down stairs immediately!" /* Zelda 3 */
	"Gary was here! Ash is a loser!" /* Pokemon */
	
	"I see you...",
		"...do you see me?",
		"That was a mistake.",
	"What was drowned, and washed away, might never rise again.",
		"Eternity is a lie.",
		"Everything dies.",
		"What is lost might not be found.",
		"And even if it all came back, it would never be the same.",
	
	"Department of Game Balance: Closing barn doors since day two.",
	"Department of Game Balance, DNetHack Branch Office: Designing well-balanced elder priests for you to enjoy fighting!",
	
	"Listen at us, won't you?", /* All Our Friends Are Dead */
		"Share with us your head",
		"Do hide behind the iris",
		"As all of our friends are dead.",
	
	"T1M3 FOr WH4T?", /*Homestuck. This is Terezi's side of her conversation with Snowman, */
		"WH4T M1SS1ON?", /*in which Terezi communicated by writing on walls and statues etc with colored chalk*/
		"WH4T 4BOUT TH3 QU33N?",
		"J4CK W4S H3LP1NG US 3X1L3 H3R SO...BOTH??? >:\\",
		"OK H3H3H3 >:] WOW OHMYGOD D4RK T3XT L4DY! 1 G3T 1T!!!!!",
		">8[ NO 1 DONT TH1NK SO WH3R3 4R3 YOU TH3N?",
		"OK SORRY 1 4SK3D! >:O",
		"WH4T? NO H3H3H3 1TS OK B3C4US3",
		"1 DONT W4NT TO RUN OUT OF R3D >:[",
		"B3C4US3!!! R3D 1S TH3 B3ST 4ND T4ST3S TH3 B3ST!!! <3",
		"OK TH3N WH4T?",
		"WH3R3 1S 1T?",
	"Im Sure That Possibly All Made Sense Good Speech",
	
	"And there shall be laughter and magic and blood, and we shall dance our dance until the end of time...", /*Incursion, Halls of the Goblin King*/
		"May you find beauty in endings.",
	
	"The eclipse will shatter the sky and from the remnants will bleed a storm of fire to engulf the earth.",	/* Gone with the Blastwave */
		"The spirits of past sins will char the land and cleanse the planet.",
		"As humans shed their former selves their true form shall be revealed.",
		"Join us in the dancing lights. Join us in the Celebration!",
		"Burn them! Burn ALL!",
		
	"Speak not the Watchers.",	/* Drakengard */
		"Draw not the Watchers.",
		"Write not the Watchers.",
		"Sculpt not the Watchers.",
		"Sing not the Watchers.",
		"Call not the Watchers' name.",
	"Should you encounter God on your quest, God will be cut.", /*Kill Bill*/
	"Generally, when one is conducting an experiment, it is worth noting that the observers went insane, killed each other, and then rose from the dead.", 
		/*Pyro627, regarding an experiment in creating undead bacon in Dwarf Fortress. An "accident" with an axe led to a zombified army, 
			which the poster didn't even comment on.*/
		"Today, in 'poor word choice': 'Urist, go make a new table with our mason.'", /*Swonnrr, regarding a fell mood.*/
		"The horses outnumber us. I have seen settlements with a thousand horses to a man. I have seen them in the deepest caverns. They are everywhere. Save us.", /*Quote from: Fniff regarding a bug causing ubiquitous horses*/
	"All the world's cracked but me and thee; and I'm not too sure about thee.",
	"Some walls are made of fear, but some are made from wall material.", /*Clash of Clans metro advert */
	"No gods, no masters.", /*Anarchist slogan*/
	"For the world's more full of weeping than you can understand.", /*The Stolen Child*/
	"No matter where you go, there you are.",
	"You noticed them, and they noticed that you noticed them.", /*The Mothman Prophecies*/
	"Well, how did I get here?", /*Once In A Lifetime*/
	"You can be me when I'm gone.", /*Sandman*/
		"You can stop being anything.",
		"Sometimes you wake up. Sometimes the fall kills you. And sometimes, when you fall, you fly.",
		"To absent friends, lost loves, old gods and the season of mists.",
		"And may each and everyone of us always give the devil his due.",
		"Innocence, once lost, can never be regained.",
		"In the pale light of the Moon I play the game of you. Whoever I am. Whoever you are.",
	"Have been unavoidably detained by the world. Expect us when you see us.", /* Stardust */
		"There was once a young man who wished to gain his Heart's Desire.",
		"Time, the thief, eventually takes all things into his dusty storehouse.",
		"Scuse me, but would you mind dreamin' a bit quieter?",
	"He who is valiant and pure of spirit may find the holy grail in the Castle of Aaaaarrrrrrggghhh...", /*Monty Python*/
	"If someone is reading this...I must have failed.", /*The Guardian Legend*/
	"\"Wasn't somebody's father?\"", /*The main plot point of Star Wars, according to one who hasn't seen it*/
	"The psychic efficacy of evil magic swords is vastly overrated.", /* The Water Phoenix King*/
		"It is dangerous to be right when the gods are wrong.",
		"Here are blind idiot children playing with a magic that hates them.",
		"The walled-up abomination seems friendly enough.",
		"Kill you... Take your face...",
		"The trees don't even change color anymore.",
		"What happend to you? It wasn't me!",
		"I'm sorry you could never forgive me.",
	"Reach heaven through violence.",/*Kill 6 Billion Demons*/
		"Kindly ignore my first three answers.",
	"ALL POWER TO THE OLD FLESH!!!!!!! A HOWLING VOID UPON DETRACTORS!!!", /*Awful Hospital*/
	"You are lost. You can never go home.", /* Gollum's Song */
	"But now, we dance this grim fandango, and will for years until we rest.", /* Grim Fandango */
	"Bad Wolf", /* Dr. Who */
	"It's only the end if you want it to be.", /*Batgirl*/
	"Mistakes were made.",
	"But at what cost?",
	"Live. Die. Repeat.", /*Edge of Tomorrow*/
	"For we wrestle not against flesh and blood, but against principalities and powers.", /* Ephesians 6:12 */
	"Life is truth, and never a dream...", /* Persona 4 */
	"fire, walk with me.", /*Twin Peaks*/
	"redefine happiness", /* Blue Oyster Cult */
	"Evolve. Transform. Transcend", /*Heart of the Swarm*/
	"no more sadness, no more anger, no more envy...", /*Halo*/
		"Silence fills the empty grave, now that I have gone.",
		"But my mind is not at rest, for questions linger on.",
	"Everyone you know will tell you small lies, except the ones you love, who will tell you large ones.", /*Borderlands 2*/
		"I am eternal / Death will never capture me / My skills exceed his.",
	"We are born of the blood.",/*Bloodborne*/
		"Made men by the blood.",
		"Undone by the blood.",
		"Fear the old blood.",
		"The Great Ones are sympathetic in spirit, and often answer when called upon.",
		"O Flora, of the moon, of the dream.",
		"O Little ones, o fleeting will of the ancients.",
		"Let the hunter be safe.  Let this dream foretell a pleasant awakening...",
	"We do not breathe. We do not eat. We do not sleep. We do not stop. ", /* Sigma */
		"We have outlasted our gods. We know true freedom.",
		"We do not suffer the enslavement of your passions, or your religions, or your creeds and your self conflict.",
		"We are the Vederiction of our fallen deities, their visage perfected.",
		"We have marched upon the cosmos long before your ancestors left their home.",
		"And we will endure long after your dying civilization perishes on the chain of worlds.",
	"Even false things are true",	"FIVE TONS OF FLAX!",	"Fnord", /*Discordianism*/
	"Ash nazg durbatuluk, ash nazg gimbatul, ash nazg thrakatuluk, agh burzum-ishi krimpatul", /*the Lord of the Rings*/
		"This thing all things devours.",
		"Not all those who wander are lost.",
	"This is the curse of the Buddha-you will never again be the same as once you were.", /*Lord of Light*/
		"Tricky and brilliant and heartfelt and dangerous.",
		"Death is mighty, and is no one's friend.",
		"It is the difference between the unknown and the unknowable, between science and fantasy-it is a matter of essence.",
		"Go away. This is not a place to be.",
	"No Fate", /*Terminator*/
	"When the world is in darkness Four Warriors will come....", /*FF I*/
	"This world can have but one Emperor.", /*FF II*/
	"We shall devour your light, and use it to return this world to the Void!", /*FF III*/
	"You tread the path of darkness.",/*FF IV*/
	"My hatred will not be stanched until it has consumed all else!", /*FF V*/
	"Life... dreams... hope... Where do they come from? And where do they go...?", /*FF VI*/
	"Mine is special. It's good for absolutely nothing!", /*FF VII*/
	"I will NEVER be a memory.", /*FF VII Advent Children*/
	"Trees fade with time, as do many things in this world.", /*FF VII Dirge of Cerberus*/
	"Even if the world becomes your enemy... I'll be your knight, I'll protect you.", /*FF VIII*/
	"You don't need a reason to help someone.", /*FF IX*/
	"Memories are nice, but that's all they are.", /*FF X*/
	"Better to leave things lost.", /*FF X-2*/
	"Benighted child, Unchosen, with life so short, This tower freely may you tread.", /*FF XII*/
	"The thirteen days after we awoke were the beginning of the end.", /*FF XIII*/
	"Servant of time. Daughter of chaos. Unto the world unseen, the untamed you guide.", /*FF XIII-2*/
	"One day, the light touched me.", /*FF XIII-3*/
	"What greater weapon is there than to turn an enemy to your cause? To use their own knowledge against them?", /* Knights of the Old Republic */
		"Sand stays.  All else changes.",
		"We will remember you when you are sand too.",
	"That is a lesson I will never learn, for I do not believe it.", /* Knights of the Old Republic II */
	"Do not call up any that you can not put down.", /*HP Lovecraft*/
		"That is not dead, that can eternal lie,",
		"for with strange aeons even death may die.",
	"All the world will be your enemy, Prince With A Thousand Enemies...", /*Watership Down*/
		"...and when they catch you they will kill you...",
		"but first they must catch you.",
	"This statement is false.", /*Logical contradiction*/
	"History is an account, mostly false, of events, mostly unimportant, which are brought about by rulers, mostly knaves, and soldiers, mostly fools",  /*Ambrose Bierce*/
	"Quod Erat Demonstrandum",	"anno Domini",	"per centum", /* QED, AD, percent */
	"I was so surprised, I CLASSIFIED INFORMATION, I really did!", /*Haruhi Suzumiya*/
		"Problems that cannot be solved do not exist in this world.",
		"Feelings of love are just a temporary lapse in judgment.",
	"I'm not going there to die. I'm going to find out if I'm really alive.", /*Cowboy Bebop*/
		"There is nothing left to believe in.",
	"He hides who he really is and pretends to be someone else forever.", /*FLCL*/
		"So in time he becomes that person, so his lie becomes the truth, see?  He transcends the mask.",
		"Well, don't you get it? That's how he finds happiness. That's pretty good, right?",
		"Special Containment Procedures: You're going to die, you poor dumb fuck.", /* SCP-1983 */
		"Hello.", "You are no longer lost.", "Here is the map for your journey up.", /* SCP-2084 - Anabasis */
		"A war, whoever was left alive, was Clean.", /* Red Sea Object */
		"Let the depths rise up and sweep over the shores, rendering the works of man to dust.", /* SCP-1014 - Jonah Crusoe */
		"in my head theres a sick silk nail", /* SCP 1291 */
	"I prepared Explosive Runes this morning.", /*Order of the Stick*/
		"In any battle, there's always a level of force against which no tactics can succeed.",
	"1002: He is Not that Which He Says He Is.", /*Good Omens*/
		"3001: Behinde the Eagle's Neste a grate Ash hath fellen.",
	"VI. You Shall Not Subject Your God To Market Forces!",	/*Discworld*/
		"Nae quin! Nae laird! Nae master! We willna be fooled again!",
		"Morituri Nolumus Mori",
		"End-of-the-World Switch. PLEASE DO NOT TOUCH",
		"Give a man a fire and he's warm for a day, but set fire to him and he's warm for the rest of his life.",
		"Do not, under any circumstances, open this door",
		"WORDS IN THE HEART CANNOT BE TAKEN",
		"Slab: Jus' say \"AarrghaarrghpleeassennononoUGH\"",
		"Ahahahahaha! Ahahahaha! Aahahaha! BEWARE!!!!! Yrs sincerely, The Opera Ghost",
		"Bee There Orr Bee A Rectangular Thyng",
		"We're certainly dwarves",
		"There is no doubt being human is incredibly difficult and cannot be mastered in one lifetime.",
	"NOT A Secret Base", /*Gunnerkrigg Court*/
	"We have the option to definitely die, but I've decided against it.", /*Schlock Mercenary*/
	"Short to long term memory impaired. Go to Robotics Building. Explain about Bowman's architecture. Write down everything.", /*Freefall*/
	"I SAW THE EYE. Over the horizon, like a rising sun!", /* Dresden Codak */
		"That's Science, isn't it? The perpetual horizon. But we found a way, haha, we found a way around that!",
		"What would YOU give up to... to see it through?",
		"Nothing. A shadow is nothing. It is merely a question not yet answered. We only fear the dark if we have no means of lighting our way.",
		"The world turns, and dawn comes. And under the light of the sun I shall slay giants.",
	"Don't trust the skull.", /* Cynarfpncr: Gbezrag (mild spoiler) */
	"death is the greatest illusion", /*Blade of Innocence*/
	"Our sandwiches come with attempted assassination plots. New sales gimmick!", /*Shemeska's Planescape Storyhour*/
		"I've stood at that edge! I've looked over into that void! I'm just looking at it from the bottom now!",
		"Free! Apple pie with every meal! We don't care if you don't like apple - Take It!",
		"For there is a hole in the sky",
		"and too long have I gazed",
	"The quality of your lives depends on what you make of them!", /* The Life and Times of Scrooge McDuck */
	"We do not threaten or warn or advise. We tell the truth.", /* The Mansion of E */
		"What you stole is even more dangerous and precious than you know.",
		"In the end, you will fly away and live forever, and you will leave the world shattered in your wake.",
		"You will return to where you started, and you will become what you hate.",
		"And in doing so, you will restore the world to what it was.",
		">^ N^T R3A> T||1S S1<N",
		">^ R3A> T||1S ^N3",
	"Fall in a more hardboiled manner.", /* MS Paint Adventures */
		"Since you are reading this, chances are you have already installed this game on your computer.  If this is true, you have just participated in bringing about the end of the world.  But don't beat yourself up about it.",
		"YOU HATE TIME TRAVEL YOU HATE TIME TRAVEL YOU HATE....",
		"It begins to dawn on you that everything you just did may have been a colossal waste of time.",
	"We will BUILD heroes!", /* THE PROTOMEN */
		"If you replace the working parts, you get a different machine.",
		"If you destroy the working parts, what you get is a broken machine.",
	"To right the countless wrongs of our day, we shine this light of true redemption, that this place may become as paradise.",/*It's a Wonderful World/The World Ends with You*/
		"What a wonderful world such would be...",
		"Please...Don't kill me...",
		"Another world awaits. ...And you're going!",
		"You have seven days.",
	"But the future refused to change.", /*Chrono Trigger*/
	"WHEN ALL ELSE FAILS USE FIRE",
	"Don't you see? All of you... YOUR GODS DESTROYED YOU",
		"The wind... It is blowing...",
	"I wonder... If you do the right thing... Does it really make... everybody... happy?",/*Majora's Mask*/
		"Your friends... What kind of... people are they? I wonder... Do these people... think of you... as a friend?",
		"What makes you happy? I wonder...what makes you happy...does it make...others happy, too?",
		"Your true face... What kind of... face is it? I wonder... The face under the mask... Is that... your true face?",
		"You've met with a terrible fate, haven't you?",
	"Why are you sad?", /*Blood-C*/
	"Only accurate information has practical application.", /* not sure where this originated, possibly Aron Ra?*/
	"We will come for you again  When you are alone  In the dark  Without your sword", /*Jake Wyatt's Necropolis*/
		"My sword is only steel in a useful shape. It's me you should fear, you stupid, dead things.",
		"There are horrors.  But also the sun, the grass... And the road I wish to travel.",
	"Now watch what you say or they'll be calling you a radical!", /* The Logical Song, Supertramp */
	"I took a walk around the world, to ease my troubled mind.", /* Kryptonite */
	"Do what thou wilt shall be the whole of the Law", /* Aleister Crowley */
	"Oh, 'great warrior...' Wars not make one great!", /*Star Wars*/
		"I've got a bad feeling about this...",
		"Do, or Do Not.  There is no Try.",
		"Never tell me the odds!"

};

const char *
random_mesgX()
{
	return (random_mesg[rn2(SIZE(random_mesg))]);
}

char *
random_engraving(outbuf)
char *outbuf;
{
	const char *rumor;

	/* a random engraving may come from the "rumors" file,
	   or from the list above */
	if (!rn2(5)) strcpy(outbuf, fauxmessage()); /* sometimes have faux messages --Amy */
	else if (!rn2(3) || !(rumor = getrumor(0, outbuf, TRUE)) || !*rumor) /* increased random rumor chance --Amy */
	    strcpy(outbuf, random_mesg[rn2(SIZE(random_mesg))]);

	if (!rn2(5)) wipeout_text(outbuf, (int)(strlen(outbuf) / (8 + rn2(5)) ), 0); /* wipe out less, idea by Soviet5lo */
	return outbuf;
}

/* Partial rubouts for engraving characters. -3. */
static const struct {
	char		wipefrom;
	const char *	wipeto;
} rubouts[] = {
	{'A', "^"},     {'B', "Pb["},   {'C', "("},     {'D', "|)["},
	{'E', "|FL[_"}, {'F', "|-"},    {'G', "C("},    {'H', "|-"},
	{'I', "|"},     {'K', "|<"},    {'L', "|_"},    {'M', "|"},
	{'N', "|\\"},   {'O', "C("},    {'P', "F"},     {'Q', "C("},
	{'R', "PF"},    {'T', "|"},     {'U', "J"},     {'V', "/\\"},
	{'W', "V/\\"},  {'Z', "/"},
	{'b', "|"},     {'d', "c|"},    {'e', "c"},     {'g', "c"},
	{'h', "n"},     {'j', "i"},     {'k', "|"},     {'l', "|"},
	{'m', "nr"},    {'n', "r"},     {'o', "c"},     {'q', "c"},
	{'w', "v"},     {'y', "v"},
	{':', "."},     {';', ","},
	{'0', "C("},    {'1', "|"},     {'6', "o"},     {'7', "/"},
	{'8', "3o"}
};

void
wipeout_text(engr, cnt, seed)
char *engr;
int cnt;
unsigned seed;		/* for semi-controlled randomization */
{
	char *s;
	int i, j, nxt, use_rubout, lth = (int)strlen(engr);

	if (lth && cnt > 0) {
	    while (cnt--) {
		/* pick next character */
		if (!seed) {
		    /* random */
		    nxt = rn2(lth);
		    use_rubout = rn2(4);
		} else {
		    /* predictable; caller can reproduce the same sequence by
		       supplying the same arguments later, or a pseudo-random
		       sequence by varying any of them */
		    nxt = seed % lth;
		    seed *= 31,  seed %= (BUFSZ-1);
		    use_rubout = seed & 3;
		}
		s = &engr[nxt];
		if (*s == ' ') continue;

		/* rub out unreadable & small punctuation marks */
		if (index("?.,'`-|_", *s)) {
		    *s = ' ';
		    continue;
		}

		if (!use_rubout)
		    i = SIZE(rubouts);
		else
		    for (i = 0; i < SIZE(rubouts); i++)
			if (*s == rubouts[i].wipefrom) {
			    /*
			     * Pick one of the substitutes at random.
			     */
			    if (!seed)
				j = rn2(strlen(rubouts[i].wipeto));
			    else {
				seed *= 31,  seed %= (BUFSZ-1);
				j = seed % (strlen(rubouts[i].wipeto));
			    }
			    *s = rubouts[i].wipeto[j];
			    break;
			}

		/* didn't pick rubout; use '?' for unreadable character */
		if (i == SIZE(rubouts)) *s = '?';
	    }
	}

	/* trim trailing spaces */
	while (lth && engr[lth-1] == ' ') engr[--lth] = 0;
}

boolean
can_reach_floor()
{
	return (boolean)(!u.uswallow &&
			/* Restricted/unskilled riders can't reach the floor */
			!(u.usteed && (PlayerCannotUseSkills || P_SKILL(P_RIDING) < P_BASIC) ) &&
			 (!Levitation || is_table(u.ux, u.uy) ||
			  Is_airlevel(&u.uz) || Is_waterlevel(&u.uz)));
}
#endif /* OVLB */
#ifdef OVL0

const char *
surface(x, y)
register int x, y;
{
	register struct rm *lev = &levl[x][y];

	if ((x == u.ux) && (y == u.uy) && u.uswallow &&
		is_animal(u.ustuck->data))
	    return "maw";
	else if (IS_AIR(lev->typ) && Is_airlevel(&u.uz))
	    return "air";
	else if (is_waterypool(x,y))
	    return (Underwater && !Is_waterlevel(&u.uz)) ? "bottom" : "water";
	else if (is_ice(x,y))
	    return "ice";
	else if (is_lava(x,y))
	    return "lava";
	else if (lev->typ == DRAWBRIDGE_DOWN)
	    return "bridge";
	else if(IS_ALTAR(levl[x][y].typ))
	    return "altar";
	else if(IS_GRAVE(levl[x][y].typ))
	    return "headstone";
	else if(IS_FOUNTAIN(levl[x][y].typ))
	    return "fountain";
	else if ((IS_ROOM(lev->typ) && !Is_earthlevel(&u.uz)) ||
		 IS_WALL(lev->typ) || IS_DOOR(lev->typ) || lev->typ == SDOOR)
	    return "floor";
	else
	    return "ground";
}

const char *
ceiling(x, y)
register int x, y;
{
	register struct rm *lev = &levl[x][y];
	const char *what;

	/* other room types will no longer exist when we're interested --
	 * see check_special_room()
	 */
	if (*in_rooms(x,y,VAULT))
	    what = "vault's ceiling";
	else if (*in_rooms(x,y,TEMPLE))
	    what = "temple's ceiling";
	else if (*in_rooms(x,y,SHOPBASE))
	    what = "shop's ceiling";
	else if (IS_AIR(lev->typ))
	    what = "sky";
	else if (Underwater)
	    what = "water's surface";
	else if ((IS_ROOM(lev->typ) && !Is_earthlevel(&u.uz)) ||
		 IS_WALL(lev->typ) || IS_DOOR(lev->typ) || lev->typ == SDOOR)
	    what = "ceiling";
	else
	    what = "rock above";

	return what;
}

struct engr *
engr_at(x, y)
xchar x, y;
{
	register struct engr *ep = head_engr;

	while(ep) {
		if(x == ep->engr_x && y == ep->engr_y)
			return(ep);
		ep = ep->nxt_engr;
	}
	return((struct engr *) 0);
}

/* Decide whether a particular string is engraved at a specified
 * location; a case-insensitive substring match used.
 * Ignore headstones, in case the player names herself "Elbereth".
 */
int
sengr_at(s, x, y)
	const char *s;
	xchar x, y;
{
	register struct engr *ep = engr_at(x,y);

	return (ep && ep->engr_type != HEADSTONE &&
		ep->engr_time <= moves && strstri(ep->engr_txt, s) != 0);
}

#endif /* OVL0 */
#ifdef OVL2

void
u_wipe_engr(cnt)
register int cnt;
{
	if (can_reach_floor())
		wipe_engr_at(u.ux, u.uy, cnt);
}

#endif /* OVL2 */
#ifdef OVL1

void
wipe_engr_at(x,y,cnt)
register xchar x,y,cnt;
{
	register struct engr *ep = engr_at(x,y);

	/* Headstones are indelible */
	if(ep && ep->engr_type != HEADSTONE){
	    if(ep->engr_type != BURN || is_ice(x,y)) {
		if(ep->engr_type != DUST && ep->engr_type != ENGR_BLOOD) {
			cnt = /*rn2(1 + 40/(cnt+1)) ? 0 : 1)*/rnd(cnt) ; /* nerf to semi-permanent Elbereths --Amy */
			if (rn2(5)) cnt /= 2;
		}
		wipeout_text(ep->engr_txt, (int)cnt, 0);
		while(ep->engr_txt[0] == ' ')
			ep->engr_txt++;
		if(!ep->engr_txt[0]) del_engr(ep);
	    }
	}

      if (ep && ep->engr_type == BURN && sengr_at("Elbereth", x, y) && !rn2(10000) ) {

	/* Burned Elbereth engravings will no longer be truly permanent. --Amy */

		pline(Hallucination ? "Flaming words rising up into the air... this must be MAGIC!" : "Suddenly, the burned engraving disappears into the ether!");
	    del_engr(ep);
	    ep = (struct engr *)0;

	}

      if (sobj_at(SCR_SCARE_MONSTER, x, y) && !rn2(issoviet ? 1000 : 50) ) {

	/* scare monster scrolls might randomly disappear --Amy */

		if (!rn2(10)) pline(issoviet ? "Vy chuvstvuyete nekotoruyu ekstremal'nuyu zharu! Popytka Soveta SSSR sokhranit' ispug monstr svitkov obratnyy effekt, kha-kha-kha!" : Hallucination ? "You get a hot feeling! Maybe some sexy girls are around here somewhere?" : "You sense some sort of heat for a moment!");
		(void) burn_floor_paper(x, y, TRUE, FALSE); /* The actual chance of this removing the scroll is about 3%. */

	}



}

#endif /* OVL1 */
#ifdef OVL2

boolean
sense_engr_at(x,y,read_it)
register int x,y;
boolean read_it; /* Read any sensed engraving */
{
	register struct engr *ep = engr_at(x,y);
	register int	sensed = 0;
	char buf[BUFSZ];
	
	/* Sensing an engraving does not require sight,
	 * nor does it necessarily imply comprehension (literacy).
	 */
	if(ep && ep->engr_txt[0]) {
	    switch(ep->engr_type) {
	    case DUST:
		if(!Blind) {
			sensed = 1;
			pline("%s is written here in the %s.", Something,
				is_ice(x,y) ? "frost" : "dust");
		}
		break;
	    case ENGRAVE:
	    case HEADSTONE:
		if (!Blind || can_reach_floor()) {
			sensed = 1;
			pline("%s is engraved here on the %s.",
				Something,
				surface(x,y));
		}
		break;
	    case BURN:
		if (!Blind || can_reach_floor()) {
			sensed = 1;
			pline("Some text has been %s into the %s here.",
				is_ice(x,y) ? "melted" : "burned",
				surface(x,y));
		}
		break;
	    case MARK:
		if(!Blind) {
			sensed = 1;
			pline("There's some graffiti on the %s here.",
				surface(x,y));
		}
		break;
	    case ENGR_BLOOD:
		/* "It's a message!  Scrawled in blood!"
		 * "What's it say?"
		 * "It says... `See you next Wednesday.'" -- Thriller
		 */
		if(!Blind) {
			sensed = 1;
			You("see a message scrawled in blood here.");
		}
		break;
	    default:
		impossible("%s is written in a very strange way.",
				Something);
		sensed = 1;
	    }
	    if (sensed && !read_it &&
			    flags.suppress_alert < FEATURE_NOTICE_VER(0,0,7)) {
		pline("Use \"r.\" to read it.");
	    } else if (sensed && read_it) {
	    	char *et;
	    	unsigned len, maxelen = BUFSZ - sizeof("You feel the words: \"\". ");
	    	len = strlen(ep->engr_txt);
	    	if (len > maxelen) {
	    		(void)strncpy(buf,  ep->engr_txt, (int)maxelen);
			buf[maxelen] = '\0';
			et = buf;
		} else
			et = ep->engr_txt;

		/* If you can engrave an 'x', you can "read" it --ALI */
		if (len != 1 || (!index(et, 'x') && !index(et, 'X')))
			u.uconduct.literate++;

		You("%s: \"%s\".",
		      (Blind) ? "feel the words" : "read",  et);
		if(flags.run > 1) nomul(0, 0);
		return TRUE;
	    }
	}
	return FALSE;
}

#endif /* OVL2 */
#ifdef OVLB

void
make_engr_at(x,y,s,e_time,e_type)
register int x,y;
register const char *s;
register long e_time;
register xchar e_type;
{
	register struct engr *ep;

	if ((ep = engr_at(x,y)) != 0)
	    del_engr(ep);
	ep = newengr(strlen(s) + 1);
	ep->nxt_engr = head_engr;
	head_engr = ep;
	ep->engr_x = x;
	ep->engr_y = y;
	ep->engr_txt = (char *)(ep + 1);
	strcpy(ep->engr_txt, s);
	/* engraving Elbereth shows wisdom */
	if (!in_mklev && !strcmp(s, "Elbereth")) exercise(A_WIS, TRUE);
	ep->engr_time = e_time;
	ep->engr_type = e_type > 0 ? e_type : rnd(N_ENGRAVE-1);
	ep->engr_lth = strlen(s) + 1;
}

/* delete any engraving at location <x,y> */
void
del_engr_at(x, y)
int x, y;
{
	register struct engr *ep = engr_at(x, y);

	if (ep) del_engr(ep);
}

/*
 *	freehand - returns true if player has a free hand
 */
int
freehand()
{
	return(!uwep || !welded(uwep) ||
	   (!bimanual(uwep) && (!uarms || !uarms->cursed)));
/*	if ((uwep && bimanual(uwep)) ||
	    (uwep && uarms))
		return(0);
	else
		return(1);*/
}

static NEARDATA const char styluses[] =
	{ ALL_CLASSES, ALLOW_NONE, TOOL_CLASS, WEAPON_CLASS, WAND_CLASS,
	  GEM_CLASS, RING_CLASS, 0 };

/* Mohs' Hardness Scale:
 *  1 - Talc		 6 - Orthoclase
 *  2 - Gypsum		 7 - Quartz
 *  3 - Calcite		 8 - Topaz
 *  4 - Fluorite	 9 - Corundum
 *  5 - Apatite		10 - Diamond
 *
 * Since granite is a igneous rock hardness ~ 7, anything >= 8 should
 * probably be able to scratch the rock.
 * Devaluation of less hard gems is not easily possible because obj struct
 * does not contain individual oc_cost currently. 7/91
 *
 * steel     -	5-8.5	(usu. weapon)
 * diamond    - 10			* jade	     -	5-6	 (nephrite)
 * ruby       -  9	(corundum)	* turquoise  -	5-6
 * sapphire   -  9	(corundum)	* opal	     -	5-6
 * topaz      -  8			* glass      - ~5.5
 * emerald    -  7.5-8	(beryl)		* dilithium  -	4-5??
 * aquamarine -  7.5-8	(beryl)		* iron	     -	4-5
 * garnet     -  7.25	(var. 6.5-8)	* fluorite   -	4
 * agate      -  7	(quartz)	* brass      -	3-4
 * amethyst   -  7	(quartz)	* gold	     -	2.5-3
 * jasper     -  7	(quartz)	* silver     -	2.5-3
 * onyx       -  7	(quartz)	* copper     -	2.5-3
 * moonstone  -  6	(orthoclase)	* amber      -	2-2.5
 */

/* return 1 if action took 1 (or more) moves, 0 if error or aborted */
int
doengrave()
{
	boolean dengr = FALSE;	/* TRUE if we wipe out the current engraving */
	boolean doblind = FALSE;/* TRUE if engraving blinds the player */
	boolean doknown = FALSE;/* TRUE if we identify the stylus */
	boolean eow = FALSE;	/* TRUE if we are overwriting oep */
	boolean jello = FALSE;	/* TRUE if we are engraving in slime */
	boolean ptext = TRUE;	/* TRUE if we must prompt for engrave text */
	boolean teleengr =FALSE;/* TRUE if we move the old engraving */
	boolean zapwand = FALSE;/* TRUE if we remove a wand charge */
	xchar type = DUST;	/* Type of engraving made */
	char buf[ENGBUFSZ];	/* Buffer for final/poly engraving text */
	char ebuf[ENGBUFSZ];	/* Buffer for initial engraving text */
	char qbuf[QBUFSZ];	/* Buffer for query text */
	char post_engr_text[ENGBUFSZ]; /* Text displayed after engraving prompt */
	const char *everb;	/* Present tense of engraving type */
	const char *eloc;	/* Where the engraving is (ie dust/floor/...) */
	char *sp;		/* Place holder for space count of engr text */
	int len;		/* # of nonspace chars of new engraving text */
	int maxelen;		/* Max allowable length of engraving text */
	struct engr *oep = engr_at(u.ux,u.uy);
				/* The current engraving */
	struct obj *otmp;	/* Object selected with which to engrave */
	char *writer;

	multi = 0;		/* moves consumed */
	nomovemsg = (char *)0;	/* occupation end message */

	buf[0] = (char)0;
	ebuf[0] = (char)0;
	post_engr_text[0] = (char)0;
	maxelen = ENGBUFSZ; /* a maximum of 31 active Elbereths on a single square --Amy */
	if (is_demon(youmonst.data) || youmonst.data->mlet == S_VAMPIRE)
	    type = ENGR_BLOOD;

	/* Can the adventurer engrave at all? */

	if(u.uswallow) {
		if (is_animal(u.ustuck->data)) {
			pline("What would you write?  \"Jonah was here\"?");
			if (flags.moreforced && !(MessageSuppression || u.uprops[MESSAGE_SUPPRESSION_BUG].extrinsic || have_messagesuppressionstone() )) display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
			return(0);
		} else if (is_whirly(u.ustuck->data)) {
			You_cant("reach the %s.", surface(u.ux,u.uy));
			if (flags.moreforced && !(MessageSuppression || u.uprops[MESSAGE_SUPPRESSION_BUG].extrinsic || have_messagesuppressionstone() )) display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
			return(0);
		} else
			jello = TRUE;
	} else if (is_lava(u.ux, u.uy)) {
		You_cant("write on the lava!");
		if (flags.moreforced && !(MessageSuppression || u.uprops[MESSAGE_SUPPRESSION_BUG].extrinsic || have_messagesuppressionstone() )) display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
		return(0);
	} else if (is_styxriver(u.ux, u.uy)) {
		You_cant("write on the styx river!");
		if (flags.moreforced && !(MessageSuppression || u.uprops[MESSAGE_SUPPRESSION_BUG].extrinsic || have_messagesuppressionstone() )) display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
		return(0);
	} else if (is_shiftingsand(u.ux, u.uy)) {
		You_cant("write on the shifting sand!");
		if (flags.moreforced && !(MessageSuppression || u.uprops[MESSAGE_SUPPRESSION_BUG].extrinsic || have_messagesuppressionstone() )) display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
		return(0);
	} else if (is_waterypool(u.ux,u.uy) || is_watertunnel(u.ux,u.uy) || is_moorland(u.ux,u.uy) || IS_FOUNTAIN(levl[u.ux][u.uy].typ)) {
		You_cant("write on the water!");
		if (flags.moreforced && !(MessageSuppression || u.uprops[MESSAGE_SUPPRESSION_BUG].extrinsic || have_messagesuppressionstone() )) display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
		return(0);
	}
	if(Is_airlevel(&u.uz) || Is_waterlevel(&u.uz)/* in bubble */) {
		You_cant("write in thin air!");
		if (flags.moreforced && !(MessageSuppression || u.uprops[MESSAGE_SUPPRESSION_BUG].extrinsic || have_messagesuppressionstone() )) display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
		return(0);
	}
	if(IS_AIR(levl[u.ux][u.uy].typ)) {
		You_cant("write on an unsolid surface!");
		if (flags.moreforced && !(MessageSuppression || u.uprops[MESSAGE_SUPPRESSION_BUG].extrinsic || have_messagesuppressionstone() )) display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
		return(0);
	}
	if (cantwield(youmonst.data) && !Race_if(PM_TRANSFORMER) ) {
		You_cant("even hold anything!");

		if (yn("Do you want to try engraving anyway?") == 'y') {
			if (rn2(3)) { 		
			pline("You failed to engrave anything.");
		    return 1;}
		}

		/* Just make them lose that turn. If they're surrounded by 20 killer bees and 10 large cats,
		   losing a turn while engraving might be enough to kill them anyway. No need for further penalties. --Amy */
		else {return(0);}
	}
	if (check_capacity((char *)0)) return (0);

	/* One may write with finger, or weapon, or wand, or..., or...
	 * Edited by GAN 10/20/86 so as not to change weapon wielded.
	 */

	otmp = getobj(styluses, "write with");
	if(!otmp) return(0);		/* otmp == zeroobj if fingers */

	if (otmp == &zeroobj) writer = makeplural(body_part(FINGER));
	else writer = xname(otmp);

	/* There's no reason you should be able to write with a wand
	 * while both your hands are tied up.
	 */
	if (!freehand() && otmp != uwep && !otmp->owornmask) {
		You("have no free %s to write with!", body_part(HAND));
		if (flags.moreforced && !(MessageSuppression || u.uprops[MESSAGE_SUPPRESSION_BUG].extrinsic || have_messagesuppressionstone() )) display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
		return(0);
	}

	if (jello) {
		You("tickle %s with your %s.", mon_nam(u.ustuck), writer);
		Your("message dissolves...");
		if (flags.moreforced && !(MessageSuppression || u.uprops[MESSAGE_SUPPRESSION_BUG].extrinsic || have_messagesuppressionstone() )) display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
		return(0);
	}
	if (otmp->oclass != WAND_CLASS && !can_reach_floor()) {
		You_cant("reach the %s!", surface(u.ux,u.uy));
		if (flags.moreforced && !(MessageSuppression || u.uprops[MESSAGE_SUPPRESSION_BUG].extrinsic || have_messagesuppressionstone() )) display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
		return(0);
	}
	if (IS_ALTAR(levl[u.ux][u.uy].typ)) {
		You("make a motion towards the altar with your %s.", writer);
		if (flags.moreforced && !(MessageSuppression || u.uprops[MESSAGE_SUPPRESSION_BUG].extrinsic || have_messagesuppressionstone() )) display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
		altar_wrath(u.ux, u.uy);
		return(0);
	}
	if (IS_GRAVE(levl[u.ux][u.uy].typ)) {
	    if (otmp == &zeroobj) { /* using only finger */
		You("would only make a small smudge on the %s.",
			surface(u.ux, u.uy));
		if (flags.moreforced && !(MessageSuppression || u.uprops[MESSAGE_SUPPRESSION_BUG].extrinsic || have_messagesuppressionstone() )) display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
		return(0);
	    } else if (!levl[u.ux][u.uy].disturbed) {
		You("disturb the undead!");
		if (flags.moreforced && !(MessageSuppression || u.uprops[MESSAGE_SUPPRESSION_BUG].extrinsic || have_messagesuppressionstone() )) display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
		levl[u.ux][u.uy].disturbed = 1;
		(void) makemon(&mons[PM_GHOUL], u.ux, u.uy, NO_MM_FLAGS);
		exercise(A_WIS, FALSE);
		return(1);
	    }
	}

	/* SPFX for items */

	switch (otmp->oclass) {
	    default:
	    case AMULET_CLASS:
	    case CHAIN_CLASS:
	    case POTION_CLASS:
	    case COIN_CLASS:
		break;

	    case RING_CLASS:
		/* "diamond" rings and others should work */
	    case GEM_CLASS:
		/* diamonds & other hard gems should work */
		if (objects[otmp->otyp].oc_tough) {
			type = ENGRAVE;
			break;
		}
		break;

	    case ARMOR_CLASS:
		if (is_boots(otmp)) {
			type = DUST;
			break;
		}
		/* fall through */
	    /* Objects too large to engrave with */
	    case BALL_CLASS:
	    case ROCK_CLASS:
		You_cant("engrave with such a large object!");
		ptext = FALSE;
		break;

	    /* Objects too silly to engrave with */
	    case FOOD_CLASS:
	    case SCROLL_CLASS:
	    case SPBOOK_CLASS:
		Your("%s would get %s.", xname(otmp),
			is_ice(u.ux,u.uy) ? "all frosty" : "too dirty");
		ptext = FALSE;
		break;

	    case RANDOM_CLASS:	/* This should mean fingers */
		break;

	    /* The charge is removed from the wand before prompting for
	     * the engraving text, because all kinds of setup decisions
	     * and pre-engraving messages are based upon knowing what type
	     * of engraving the wand is going to do.  Also, the player
	     * will have potentially seen "You wrest .." message, and
	     * therefore will know they are using a charge.
	     */

		/* edit by Amy - you could wrest without consuming time. Thus I won't call zappable(otmp) for empty wands.
		 * Wresting must be done by actually zapping the wand now, which will always take time. */

	    case WAND_CLASS:
		if (otmp->spe > 0) {
		if (zappable(otmp)) {
		    check_unpaid(otmp);
		    zapwand = TRUE;
		    if (Levitation) ptext = FALSE;

		    switch (otmp->otyp) {
		    /* DUST wands */
		    default:
			break;

			/* NODIR wands */
		    case WAN_LIGHT:
		    case WAN_SECRET_DOOR_DETECTION:
		    case WAN_TRAP_DISARMING:
		    case WAN_CREATE_MONSTER:
		    case WAN_CREATE_FAMILIAR:
		    case WAN_SUMMON_UNDEAD:
		    case WAN_CREATE_HORDE:
		    case WAN_ENLIGHTENMENT:
		    case WAN_ENTRAPPING:
		    case WAN_TELE_LEVEL:
		    case WAN_GENOCIDE:
		    case WAN_STINKING_CLOUD:
		    case WAN_TIME_STOP:
		    case WAN_MAGIC_MAPPING:
		    case WAN_DARKNESS:
		    case WAN_TRAP_CREATION:
		    case WAN_SUMMON_SEXY_GIRL:
		    case WAN_MANA:
		    case WAN_GAIN_LEVEL:
		    case WAN_BAD_EFFECT:
		    case WAN_DETECT_MONSTERS:
		    case WAN_OBJECTION:
		    case WAN_IDENTIFY:
		    case WAN_LEVITATION:
		    case WAN_DEBUGGING:
		    case WAN_SPELLBINDER:
		    case WAN_INERTIA_CONTROL:
		    case WAN_STERILIZE:
		    case WAN_REMOVE_CURSE:
		    case WAN_PUNISHMENT:
		    case WAN_WISHING:
		    case WAN_ACQUIREMENT:
		    case WAN_BUGGING:
		    case WAN_CHARGING:
		    case WAN_CURSE_ITEMS:
		    case WAN_AMNESIA:
		    case WAN_BAD_LUCK:
		    case WAN_REMOVE_RESISTANCE:
		    case WAN_CORROSION:
		    case WAN_FUMBLING:
		    case WAN_TIDAL_WAVE:
		    case WAN_SUMMON_ELM:
		    case WAN_STARVATION:
		    case WAN_CONFUSION:
		    case WAN_SLIMING:
		    case WAN_DRAIN_MANA:
		    case WAN_FINGER_BENDING:
		    case WAN_LYCANTHROPY:
		    case WAN_IMMOBILITY:
		    case WAN_EGOISM:
		    case WAN_SIN:
			zapnodir(otmp);
			break;

			/* IMMEDIATE wands */
			/* If wand is "IMMEDIATE", remember to affect the
			 * previous engraving even if turning to dust.
			 */
		    case WAN_STRIKING:
			strcpy(post_engr_text,
			"The wand unsuccessfully fights your attempt to write!"
			);
			break;
		    case WAN_DISINTEGRATION:
		    case WAN_DISINTEGRATION_BEAM:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s cease to exist!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_GRAVITY_BEAM:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs are driven into the %s!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_STONING:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s turn into gravel!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_DREAM_EATER:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s seem unaffected.", /* because they're not asleep --Amy */
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_BUBBLEBEAM:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s are covered with bubbles!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_GOOD_NIGHT:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s look very drowsy!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_STUN_MONSTER:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s start moving in strange patterns!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_ACID:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s seem to be covered with goo!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_SLUDGE:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s seem to be covered with litter!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_DESLEXIFICATION:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s seem unaffected.",
				   surface(u.ux, u.uy));
		/* bugs on the floor are not a slex feature, and therefore the wand does nothing to them --Amy */
			}
			break;
		    case WAN_WIND:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the floor are blown away!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_SOLAR_BEAM:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s shine brightly for a moment!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_AURORA_BEAM:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s are illuminated with rainbow colors!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_PSYBEAM:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s seem utterly confused!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_NETHER_BEAM:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s are killed!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_HYPER_BEAM:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s are vaporized!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_SLOW_MONSTER:
		    case WAN_INERTIA:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s slow down!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_SPEED_MONSTER:
		    case WAN_HASTE_MONSTER:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s speed up!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_HEALING:
		    case WAN_EXTRA_HEALING:
		    case WAN_FULL_HEALING:
		    case WAN_INCREASE_MAX_HITPOINTS:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s look healthier!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_FEAR:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s run away!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_MAKE_VISIBLE:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s are clearly visible!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_POLYMORPH:
		    case WAN_MUTATION:
			if(oep)  {
			    if (!Blind) {
				type = (xchar)0;	/* random */
				(void) random_engraving(buf);
			    }
			    dengr = TRUE;
			}
			break;
		    case WAN_DRAINING:	/* KMH */
		    case WAN_TIME:
		    case WAN_REDUCE_MAX_HITPOINTS:
			if (oep) {
			    /*
			     * [ALI] Wand of draining give messages like
			     * either polymorph or cancellation/make
			     * invisible depending on whether the
			     * old engraving is completely wiped or not.
			     * Note: Blindness has slightly different
			     * effect than with wand of polymorph.
			     */
			    u_wipe_engr(5);
			    oep = engr_at(u.ux,u.uy);
			    if (!Blind) {
				if (!oep)
				    pline_The("engraving on the %s vanishes!",
				      surface(u.ux,u.uy));
				else {
				    strcpy(buf, oep->engr_txt);
				    dengr = TRUE;
				}
			    }
			}
			break;
		    case WAN_CLONE_MONSTER:
			if (oep) {
			    if (!Blind)
				    pline_The("engraving is blurred, and you think you can see it twice!");
			    }
			break;
		    case WAN_NOTHING:
		    case WAN_UNDEAD_TURNING:
		    case WAN_OPENING:
		    case WAN_LOCKING:
		    case WAN_PROBING:
		    case WAN_MISFIRE: /* fire or lightning message in FHS... but I decided to be cruel :-P --Amy */
			break;

		    case WAN_VENOM_SCATTERING:

			if (!Blind) pline("A venom leaves the wand top.");

			break;

			/* RAY wands */
		    case WAN_MAGIC_MISSILE:
		    case WAN_SHARE_PAIN:
			ptext = TRUE;
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The %s is riddled by bullet holes!",
				   surface(u.ux, u.uy));
			}
			break;

		    /* can't tell sleep from death - Eric Backus */
		    case WAN_SLEEP:
		    case WAN_CHLOROFORM:
		    case WAN_DEATH:
		    case WAN_PARALYSIS:
			if (!Blind) {
			   sprintf(post_engr_text,
				   "The bugs on the %s stop moving!",
				   surface(u.ux, u.uy));
			}
			break;

		    case WAN_COLD:
			if (!Blind)
			    strcpy(post_engr_text,
				"A few ice cubes drop from the wand.");
			if(!oep || (oep->engr_type != BURN))
			    break;
		    case WAN_ICE_BEAM:
			if (!Blind)
			    strcpy(post_engr_text,
				"A steady stream of ice shoots from the wand.");
			if(!oep || (oep->engr_type != BURN))
			    break;
		    case WAN_CANCELLATION:
		    case WAN_MAKE_INVISIBLE:
		    case WAN_BANISHMENT:
			if (oep && oep->engr_type != HEADSTONE) {
			    if (!Blind)
				pline_The("engraving on the %s vanishes!",
					surface(u.ux,u.uy));
			    dengr = TRUE;
			}
			break;
		    case WAN_TELEPORTATION:
			if (oep && oep->engr_type != HEADSTONE) {
			    if (!Blind)
				pline_The("engraving on the %s vanishes!",
					surface(u.ux,u.uy));
			    teleengr = TRUE;
			}
			break;

		    /* type = ENGRAVE wands */
		    case WAN_DIGGING:
			ptext = TRUE;
			type  = ENGRAVE;
			if(!objects[otmp->otyp].oc_name_known) {
			    if (flags.verbose)
				pline("This %s is a wand of digging!",
				xname(otmp));
			    doknown = TRUE;
			}
			if (!Blind)
			    strcpy(post_engr_text,
				IS_GRAVE(levl[u.ux][u.uy].typ) ?
				"Chips fly out from the headstone." :
				is_ice(u.ux,u.uy) ?
				"Ice chips fly up from the ice surface!" :
				"Gravel flies up from the floor.");
			else
			    strcpy(post_engr_text, "You hear drilling!");
			break;

		    case WAN_POISON:	/*new wand [Sakusha]*/
			ptext = TRUE;
			type  = MARK;
			if(!objects[otmp->otyp].oc_name_known) {
			    if (flags.verbose)
				pline("This %s is a wand of poison!", xname(otmp));
			    doknown = TRUE;
			}
			break;

		    case WAN_TOXIC:	/*new wand [Sakusha]*/
			ptext = TRUE;
			type  = MARK;
			if(!objects[otmp->otyp].oc_name_known) {
			    if (flags.verbose)
				pline("This %s is a wand of toxic!", xname(otmp));
			    doknown = TRUE;
			}
			break;

		    /* type = BURN wands */
		    case WAN_FIRE:
			ptext = TRUE;
			type  = BURN;
			if(!objects[otmp->otyp].oc_name_known) {
			if (flags.verbose)
			    pline("This %s is a wand of fire!", xname(otmp));
			    doknown = TRUE;
			}
			strcpy(post_engr_text,
				Blind ? "You feel the wand heat up." :
					"Flames fly from the wand.");
			break;
		    case WAN_INFERNO:
			ptext = TRUE;
			type  = BURN;
			if(!objects[otmp->otyp].oc_name_known) {
			if (flags.verbose)
			    pline("This %s is a wand of inferno!", xname(otmp));
			    doknown = TRUE;
			}
			strcpy(post_engr_text,
				Blind ? "You feel the wand heat up." :
					"Flames fly from the wand.");
			break;
		    case WAN_CHROMATIC_BEAM:
			ptext = TRUE;
			type  = BURN;
			if(!objects[otmp->otyp].oc_name_known) {
			if (flags.verbose)
			    pline("This %s is a wand of chromatic beam!", xname(otmp));
			    doknown = TRUE;
			}
			strcpy(post_engr_text,
				Blind ? "You feel the wand trembling while you engrave." :
					"Multicolored beams fly from the wand.");
			break;
		    case WAN_FIREBALL:
			ptext = TRUE;
			type  = BURN;
			if(!objects[otmp->otyp].oc_name_known) {
			if (flags.verbose)
			    pline("This %s is a wand of fireballs!", xname(otmp));
			    doknown = TRUE;
			}
			strcpy(post_engr_text,
				Blind ? "You feel the wand heat up." :
					"Flames fly from the wand.");
			break;
		    case WAN_LIGHTNING:
			ptext = TRUE;
			type  = BURN;
			if(!objects[otmp->otyp].oc_name_known) {
			    if (flags.verbose)
				pline("This %s is a wand of lightning!",
					xname(otmp));
			    doknown = TRUE;
			}
			if (!Blind) {
			    strcpy(post_engr_text,
				    "Lightning arcs from the wand.");
			    doblind = TRUE;
			} else
			    strcpy(post_engr_text, "You hear crackling!");
			break;
		    case WAN_THUNDER:
			ptext = TRUE;
			type  = BURN;
			if(!objects[otmp->otyp].oc_name_known) {
			    if (flags.verbose)
				pline("This %s is a wand of thunder!",
					xname(otmp));
			    doknown = TRUE;
			}
			if (!Blind) {
			    strcpy(post_engr_text,
				    "Lightning arcs from the wand.");
			    doblind = TRUE;
			} else
			    strcpy(post_engr_text, "You hear crackling!");
			break;

		    /* type = MARK wands */
		    /* type = ENGR_BLOOD wands */
		    }
		} else /* end if zappable */
		    if (!can_reach_floor()) {
			You_cant("reach the %s!", surface(u.ux,u.uy));
			return(0);
		    }
			break;
		} else /* end if obj->spe > 0 */
		    if (!can_reach_floor()) {
			You_cant("reach the %s!", surface(u.ux,u.uy));
			return(0);
		    }
		break;

	    case WEAPON_CLASS:
		if(is_blade(otmp)) {
		    if ((int)otmp->spe > -3)
			type = ENGRAVE;
		    else
			Your("%s too dull for engraving.", aobjnam(otmp,"are"));
		}
		break;

	    case TOOL_CLASS:
		if(otmp == ublindf) {
		    pline(
		"That is a bit difficult to engrave with, don't you think?");
		    return(0);
		}

		if (is_lightsaber(otmp)) {
		    if (otmp->lamplit) type = /*BURN*/ENGRAVE;
		    else Your("%s is deactivated!", aobjnam(otmp,"are"));
		} else
		switch (otmp->otyp)  {
		    case MAGIC_MARKER:
		    case FELT_TIP_MARKER:
			if (otmp->spe <= 0)
			    Your("marker has dried out.");
			else
			    type = MARK;
			break;
		    case TOWEL:
			/* Can't really engrave with a towel */
			ptext = FALSE;
			if (oep)
			    if ((oep->engr_type == DUST ) ||
				(oep->engr_type == ENGR_BLOOD) ||
				(oep->engr_type == MARK )) {
				if (!Blind)
				    You("wipe out the message here.");
				else
				    Your("%s %s %s.", xname(otmp),
					 otense(otmp, "get"),
					 is_ice(u.ux,u.uy) ?
					 "frosty" : "dusty");
				dengr = TRUE;
			    } else
				Your("%s can't wipe out this engraving.",
				     xname(otmp));
			else
			    Your("%s %s %s.", xname(otmp), otense(otmp, "get"),
				  is_ice(u.ux,u.uy) ? "frosty" : "dusty");
			break;
		    default:
			break;
		}
		break;

	    case VENOM_CLASS:
#ifdef WIZARD
		/*if (wizard) {*/
		    pline(Hallucination ? "That would require some huge hands as it's just smearing big letters everywhere." : "Writing a poison pen letter??"); /* why the heck was this wizard-mode only??? --Amy */
		    break;
		/*}*/
#endif
	    case ILLOBJ_CLASS:
		impossible("You're engraving with an illegal object!");
		break;
	}

	if (IS_GRAVE(levl[u.ux][u.uy].typ)) {
	    if (type == ENGRAVE || type == 0)
		type = HEADSTONE;
	    else {
		/* ensures the "cannot wipe out" case */
		type = DUST;
		dengr = FALSE;
		teleengr = FALSE;
		buf[0] = (char)0;
	    }
	}

	/* End of implement setup */

	/* Identify stylus */
	if (doknown) {
	    makeknown(otmp->otyp);
	    more_experienced(0,10);
	}

	if (teleengr) {
	    rloc_engr(oep);
	    oep = (struct engr *)0;
	}

	if (dengr) {
	    del_engr(oep);
	    oep = (struct engr *)0;
	}

	/* Something has changed the engraving here */
	if (*buf) {
	    make_engr_at(u.ux, u.uy, buf, moves, type);
	    pline_The("engraving looks different now.");
	    ptext = FALSE;
	}

	if (zapwand && (otmp->spe < 0)) {
	    pline("%s %sturns to dust.",
		  The(xname(otmp)), Blind ? "" : "glows violently, then ");
	    if (!IS_GRAVE(levl[u.ux][u.uy].typ))
		You("are not going to get anywhere trying to write in the %s with your dust.",
		    is_ice(u.ux,u.uy) ? "frost" : "dust");
	    useup(otmp);
	    ptext = FALSE;
	}

	/* Wand of tele level can crash of course. Don't allow it to engrave, then. --Amy */
	if (otmp && otmp->otyp == WAN_TELE_LEVEL) {
		ptext = FALSE;
		pline("Somehow, this wand refuses to engrave anything.");
	}

	if (!ptext) {		/* Early exit for some implements. */
	    if (otmp->oclass == WAND_CLASS && !can_reach_floor())
		You_cant("reach the %s!", surface(u.ux,u.uy));
	    return(1);
	}

	/* Special effects should have deleted the current engraving (if
	 * possible) by now.
	 */

	if (oep) {
	    register char c = 'n';

	    /* Give player the choice to add to engraving. */

	    if (type == HEADSTONE) {
		/* no choice, only append */
		c = 'y';
	    } else if ( (type == oep->engr_type) && (!Blind ||
		 (oep->engr_type == BURN) || (oep->engr_type == ENGRAVE)) ) {
		c = yn_function("Do you want to add to the current engraving?",
				ynqchars, 'y');
		if (c == 'q') {
		    pline(Never_mind);
		    return(0);
		}
	    }

	    if (c == 'n' || Blind) {

		if( (oep->engr_type == DUST) || (oep->engr_type == ENGR_BLOOD) ||
		    (oep->engr_type == MARK) ) {
		    if (!Blind) {
			You("wipe out the message that was %s here.",
			    ((oep->engr_type == DUST)  ? "written in the dust" :
			    ((oep->engr_type == ENGR_BLOOD) ? "scrawled in blood"   :
							 "written")));
			del_engr(oep);
			oep = (struct engr *)0;
		    } else
		   /* Don't delete engr until after we *know* we're engraving */
			eow = TRUE;
		} else
		    if ( (type == DUST) || (type == MARK) || (type == ENGR_BLOOD) ) {
			You(
			 "cannot wipe out the message that is %s the %s here.",
			 oep->engr_type == BURN ?
			   (is_ice(u.ux,u.uy) ? "melted into" : "burned into") :
			   "engraved in", surface(u.ux,u.uy));
			return(1);
		    } else
			if ( (type != oep->engr_type) || (c == 'n') ) {
			    if (!Blind || can_reach_floor())
				You("will overwrite the current message.");
			    eow = TRUE;
			}
	    }
	}

	eloc = surface(u.ux,u.uy);
	switch(type){
	    default:
		everb = (oep && !eow ? "add to the weird writing on" :
				       "write strangely on");
		break;
	    case DUST:
		everb = (oep && !eow ? "add to the writing in" :
				       "write in");
		eloc = (is_ice(u.ux,u.uy) ? "frost" : "dust");
		break;
	    case HEADSTONE:
		everb = (oep && !eow ? "add to the epitaph on" :
				       "engrave on");
		break;
	    case ENGRAVE:
		everb = (oep && !eow ? "add to the engraving in" :
				       "engrave in");
		break;
	    case BURN:
		everb = (oep && !eow ?
			( is_ice(u.ux,u.uy) ? "add to the text melted into" :
					      "add to the text burned into") :
			( is_ice(u.ux,u.uy) ? "melt into" : "burn into"));
		break;
	    case MARK:
		everb = (oep && !eow ? "add to the graffiti on" :
				       "scribble on");
		break;
	    case ENGR_BLOOD:
		everb = (oep && !eow ? "add to the scrawl on" :
				       "scrawl on");
		break;
	}

	/* Tell adventurer what is going on */
	if (otmp != &zeroobj)
	    You("%s the %s with %s.", everb, eloc, doname(otmp));
	else
	    You("%s the %s with your %s.", everb, eloc,
		makeplural(body_part(FINGER)));

	/* Prompt for engraving! */
	sprintf(qbuf,"What do you want to %s the %s here?", everb, eloc);
	getlin(qbuf, ebuf);

	/* Count the actual # of chars engraved not including spaces */
	len = strlen(ebuf);
	for (sp = ebuf; *sp; sp++) if (isspace(*sp)) len -= 1;

	if (len == 0 || index(ebuf, '\033')) {
	    if (zapwand) {
		if (!Blind)
		    pline("%s, then %s.",
			  Tobjnam(otmp, "glow"), otense(otmp, "fade"));
		return(1);
	    } else {
		pline(Never_mind);
		return(0);
	    }
	}

	/* A single `x' is the traditional signature of an illiterate person */
	if (len != 1 || (!index(ebuf, 'x') && !index(ebuf, 'X')))
	    u.uconduct.literate++;

	/* Mix up engraving if surface or state of mind is unsound.
	   Note: this won't add or remove any spaces. */

	/* The game is difficult enough already. Let's improve the player's chances to engrave. --Amy */

	for (sp = ebuf; *sp; sp++) {
	    if (isspace(*sp)) continue;
	    if ( (rn2(5) || !Role_if(PM_ARTIST)) && ((type == DUST || type == ENGR_BLOOD) && !(uwep && uwep->oartifact == ART_PINSELFLINSELING) && (ishaxor ? !rn2(40) : !rn2(30) ) ) ||
		    (Blind && (ishaxor ? !rn2(35) : !rn2(25)) ) || 
			(Confusion && (ishaxor ? !rn2(30) : !rn2(20)) ) ||
			(Burned && (ishaxor ? !rn2(32) : !rn2(22)) ) ||
			(Numbed && (ishaxor ? !rn2(38) : !rn2(28)) ) ||
			(Dimmed && (ishaxor ? !rn2(35) : !rn2(25)) ) ||
			(Feared && (ishaxor ? !rn2(40) : !rn2(30)) ) ||
		    (Stunned && (ishaxor ? !rn2(25) : !rn2(15)) ) ||
			(Frozen && (ishaxor ? !rn2(20) : !rn2(10)) ) ||
			(Hallucination && (ishaxor ? !rn2(28) : !rn2(18))) )
		*sp = ' ' + rnd(96 - 2);	/* ASCII '!' thru '~'
						   (excludes ' ' and DEL) */
	}

	/* Previous engraving is overwritten */
	if (eow) {
	    del_engr(oep);
	    oep = (struct engr *)0;
	}

	/* Figure out how long it took to engrave, and if player has
	 * engraved too much. Artists can engrave much faster than other characters.
	 */
	switch(type){
	    default:
		multi = -(len/(Role_if(PM_ARTIST) ? 20 : 10) );
		if (multi) nomovemsg = "You finish your weird engraving.";
		break;
	    case DUST:
		multi = -(len/(Role_if(PM_ARTIST) ? 20 : 10) );
		if (multi) nomovemsg = "You finish writing in the dust.";
		break;
	    case HEADSTONE:
	    case ENGRAVE:
		multi = -(len/(Role_if(PM_ARTIST) ? 20 : 10) );
		if (otmp->otyp == WAN_DIGGING) multi /= 5; /* otherwise engraving with them would be useless --Amy */
		if ((otmp->oclass == WEAPON_CLASS) && !stack_too_big(otmp) &&
		    ( ((otmp->otyp != ATHAME) && (otmp->otyp != MERCURIAL_ATHAME) && (!is_lightsaber(otmp))) || otmp->cursed)) {
		    multi = -len;
		    maxelen = ((otmp->spe + 3) * 2) + 1;
			/* -2 = 3, -1 = 5, 0 = 7, +1 = 9, +2 = 11
			 * Note: this does not allow a +0 anything (except
			 *	 an athame) to engrave "Elbereth" all at once.
			 *	 However, you could now engrave "Elb", then
			 *	 "ere", then "th".
			 */
		    Your("%s dull.", aobjnam(otmp, "get"));
		    if (otmp->unpaid) {
			struct monst *shkp = shop_keeper(*u.ushops);
			if (shkp) {
			    You("damage it, you pay for it!");
			    bill_dummy_object(otmp);
			}
		    }
		    if (len > maxelen) {
			multi = -maxelen;
			otmp->spe = -3;
		    } else if (len > 1)
			otmp->spe -= len >> 1;
		    else otmp->spe -= 1; /* Prevent infinite engraving */
		} else
		    if ( (otmp->oclass == RING_CLASS) ||
			 (otmp->oclass == GEM_CLASS) )
			multi = -len;
		if (multi) nomovemsg = "You finish engraving.";
		break;
	    case BURN:
		multi = -(len/(Role_if(PM_ARTIST) ? 20 : 10) );
		if (multi)
		    nomovemsg = is_ice(u.ux,u.uy) ?
			"You finish melting your message into the ice.":
			"You finish burning your message into the floor.";
		break;
	    case MARK:
		multi = -(len/(Role_if(PM_ARTIST) ? 20 : 10) );
		if ((otmp->oclass == TOOL_CLASS) &&
		    (otmp->otyp == MAGIC_MARKER || otmp->otyp == FELT_TIP_MARKER)) {
		    maxelen = (otmp->spe) * /*2*/8; /* one charge / 2 letters */ /* Amy edit - one charge per 8 letters */
		    if (len > maxelen) {
			Your("marker dries out.");
			otmp->spe = 0;
			multi = -(maxelen/10);
		    } else if (!(otmp->oartifact == ART_SCRABBLE_BABBLE) || !rn2(10))
			if (len > 7) otmp->spe -= (len / 8);
			else otmp->spe -= 1; /* Prevent infinite grafitti */
		}
		if (multi) nomovemsg = "You finish defacing the dungeon.";
		break;
	    case ENGR_BLOOD:
		multi = -(len/(Role_if(PM_ARTIST) ? 20 : 10) );
		if (multi) nomovemsg = "You finish scrawling.";
		break;
	}

	/* Chop engraving down to size if necessary */
	if (len > maxelen) {
	    for (sp = ebuf; (maxelen && *sp); sp++)
		if (!isspace((int)*sp)) maxelen--;
	    if (!maxelen && *sp) {
		*sp = (char)0;
		if (multi) nomovemsg = "You cannot write any more.";
		You("only are able to write \"%s\"", ebuf);
	    }
	}

	/* Add to existing engraving */
	if (oep) strcpy(buf, oep->engr_txt);

	(void) strncat(buf, ebuf, (ENGBUFSZ - (int)strlen(buf) - 1));

	make_engr_at(u.ux, u.uy, buf, (moves - multi), type);

	if (post_engr_text[0]) pline(post_engr_text);

	if (doblind && !resists_blnd(&youmonst)) {
	    You("are blinded by the flash!");
	    make_blinded((long)rnd(20),FALSE);
	    if (!Blind) Your(vision_clears);
	}

	return(1);
}

void
save_engravings(fd, mode)
int fd, mode;
{
	register struct engr *ep = head_engr;
	register struct engr *ep2;
	unsigned no_more_engr = 0;

	while (ep) {
	    ep2 = ep->nxt_engr;
	    if (ep->engr_lth && ep->engr_txt[0] && perform_bwrite(mode)) {
		bwrite(fd, (void *)&(ep->engr_lth), sizeof(ep->engr_lth));
		bwrite(fd, (void *)ep, sizeof(struct engr) + ep->engr_lth);
	    }
	    if (release_data(mode))
		dealloc_engr(ep);
	    ep = ep2;
	}
	if (perform_bwrite(mode))
	    bwrite(fd, (void *)&no_more_engr, sizeof no_more_engr);
	if (release_data(mode))
	    head_engr = 0;
}

void
rest_engravings(fd)
int fd;
{
	register struct engr *ep;
	unsigned lth;

	head_engr = 0;
	while(1) {
		mread(fd, (void *) &lth, sizeof(unsigned));
		if(lth == 0) return;
		ep = newengr(lth);
		mread(fd, (void *) ep, sizeof(struct engr) + lth);
		ep->nxt_engr = head_engr;
		head_engr = ep;
		ep->engr_txt = (char *) (ep + 1);	/* Andreas Bormann */
		/* mark as finished for bones levels -- no problem for
		 * normal levels as the player must have finished engraving
		 * to be able to move again */
		ep->engr_time = moves;
	}
}

void
del_engr(ep)
register struct engr *ep;
{
	if (ep == head_engr) {
		head_engr = ep->nxt_engr;
	} else {
		register struct engr *ept;

		for (ept = head_engr; ept; ept = ept->nxt_engr)
		    if (ept->nxt_engr == ep) {
			ept->nxt_engr = ep->nxt_engr;
			break;
		    }
		if (!ept) {
		    impossible("Error in del_engr?");
		    return;
		}
	}
	dealloc_engr(ep);
}

/* randomly relocate an engraving */
void
rloc_engr(ep)
struct engr *ep;
{
	int tx, ty, tryct = 200;

	do  {
	    if (--tryct < 0) return;
	    tx = rn1(COLNO-3,2);
	    ty = rn2(ROWNO);
	} while (engr_at(tx, ty) ||
		!goodpos(tx, ty, (struct monst *)0, 0));

	ep->engr_x = tx;
	ep->engr_y = ty;
}


/* Epitaphs for random headstones */
static const char *epitaphs[] = {
	"Rest in peace",
	"R.I.P.",
	"Rest In Pieces",
	"Note -- there are NO valuable items in this grave",
	"1994-1995. The Longest-Lived Hacker Ever",
	"The Grave of the Unknown Hacker",
	"We weren't sure who this was, but we buried him here anyway",
	"Sparky -- he was a very good dog",
	"Beware of Electric Third Rail",
	"Made in Taiwan",
	"Og friend. Og good dude. Og died. Og now food",
	"Beetlejuice Beetlejuice Beetlejuice",
	"Look out below!",
	"Please don't dig me up. I'm perfectly happy down here. -- Resident",
	"Postman, please note forwarding address: Gehennom, Asmodeus's Fortress, fifth lemure on the left",
	"Mary had a little lamb/Its fleece was white as snow/When Mary was in trouble/The lamb was first to go",
	"Be careful, or this could happen to you!",
	"Soon you'll join this fellow in hell! -- the Wizard of Yendor",
	"Caution! This grave contains toxic waste",
	"Sum quod eris",
	"Here lies an Atheist, all dressed up and no place to go",
	"Here lies Ezekiel, age 102.  The good die young.",
	"Here lies my wife: Here let her lie! Now she's at rest and so am I.",
	"Here lies Johnny Yeast. Pardon me for not rising.",
	"He always lied while on the earth and now he's lying in it",
	"I made an ash of myself",
	"Soon ripe. Soon rotten. Soon gone. But not forgotten.",
	"Here lies the body of Jonathan Blake. Stepped on the gas instead of the brake.",
	"Go away!",
	/* From SLASH'EM */
	"This old man, he played one, he played knick-knack on my thumb.",
	"Don't be daft, they couldn't hit an elephant at this dist-",
	"I wonder what deleting 'system32' does?",
	"If wizards can do the tengu shuffle, I bet tourists can do the fox-trot...",
	"Kai su, teknon?",
	"So this is what it feels like to be buried alive. - Last words of Indiana Jones 1889-1946",
	"This is what happens to you if you try to kill me - The Chromatic Dragon.",
	"Wonder what this mushroom does?...",
	"Y'all watch this!",
	"You call that a punch? I'll show you a punch--",
	"$dog",
	"$playername experienced YASD",
	"$playername took war to Team Ant and bought this fine track of land as benefits.",
	"$playername, Hand of Elbereth",
	"$playername, Killed at the hands of the Random Number God.",
	"$playername, demonstrating the value of not being seen.",
	"$playername, killed by a time-travel accident",
	"$playername, killed by an ambush, while reading",
	"$playername, killed by one-more-hit syndrome",
	"'Tis but a scratch!",
	"'Tis just a flesh wound...",
	"(This space for sale.)",
	"*Reserved for You*",
	"--More--",
	"...",
	"...for famous men have the whole earth as their memorial",
	"0AJQp",
	"0B6x8",
	"0EfGr",
	"0O7dW",
	"0X5mr",
	"2Bnxa",
	"2zr4S",
	"3sDlZ",
	"4ba9W",
	"4iyBA",
	"4nLCG",
	"4oY9C",
	"5cfQi",
	"5h5J0",
	"6dq9u",
	"6i9ma",
	"6q909",
	"6sEe6",
	"6z28Z",
	"7WRTy",
	"7jN66",
	"83gTA",
	"9h5Gq",
	"9qdzJ",
	"9wRBD",
	"<Privet!>!",
	"A is for Amy who fell down the stairs.",
	"A last bastion of rationality in a world where imbecility reigns supreme.",
	"A man of few words to the very end, now a man of even fewer words.",
	"A man's character is his fate",
	"A warning: Avoid the oysters at the local buffet line.",
	"ADkS1",
	"AFK",
	"ALL YOUR BASE ARE BELONG TO US",
	"ASYcW",
	"About the Zote what can be said? There was just one and now it's dead.",
	"Ach, Hans, run! It's the lhurgoyf!",
	"Acta est fabula.",
	"Ag friend. Eg good dude. Ig died. Og now Ig",
"Alas fair Death, 'twas missed in life - some peace and quiet from my wife",
"Alas, poor Bob. He almost asended. Only to fall into a pit of spikes.",
"Alas, poor Yorick!",
"Always look on the bright side of life",
"Another one bites the dust.",
"Any advice on risk management?",
"Anyone wanna switch places?",
"Apparently, you *can't* kill huge green fierce dragons with your bare hands.",
"Applaud, my friends, the comedy is finished.",
"As you are now, I once was. As I am now, you shall be. Um, unless you're immortal.",
"At last... a nice long sleep.",
"At least I didn't get eaten by $dog or $cat.",
"Ate that little packet of silica gel labelled Do Not Eat.",
"Audi Partem Alteram",
"Aut neca aut necatis eris",
"B is for Basil, assaulted by bears",
"B0spY",
"BPEsx",
"Beetlejuice Beetlejuice Beetlejuice",
"Beloved sister, devoted friend. She saved the world a lot.",
"Beware the killer bunny!",
"Blasted here by a SLASH'EM rocket.",
"Bulk deals on gravestones - Die today!",
"Burninated.",
"COME AT ME BRO",
"Casper the Friendly Boy",
"Clint Eastwood",
"Closed for inventory",
"Come on Mary, shoot! You couldn't hit the broad side of a barn!",
"Cut off the 'Do not remove under penalty of law' tag on a matress. Got death penalty.",
"CzXSY",
"David Blaine 1973 -",
"Dead Men Tell no Tales",
"Death is but a door. Time is but a window. I'll be back!",
"Death seeketh us all.",
"Deleted system32",
"Desynchronized...",
"Did you know that death rays bounce?",
"Die? [yn] (n)",
"Died by fighting what I thought that purple h was a dwarf king was actually a mind flayer.",
"Died by losing the game.",
"Died? Again? Damn.",
"Dig me up for a good time",
"Digging up this grave can create arch-lich",
"Disco - July 12, 1979",
"DjjFu",
"Do not open until Christmas",
"Do not play while drinking.",
"Don't Try",
"Don't do whatever it was I did wrong.",
"Don't even think about reaching for that pickaxe shorty! or I'll havta bite ya.",
"Don't forget to stop and smell the roses",
"Don't let this happen to you!",
"Don't try the fish",
"Dulce et decorum est pro patria mori",
"E6Cdf",
"EAT AT JOES",
"ECCPz",
"EGE6X",
"ET IN ARCADIA EGO",
"Eaten by a Grue",
"EdQji",
"Enter here for invisible zombie rave!",
"Fatty and skinny went to bed. Fatty rolled over and skinny was dead. Skinny Smith 1983-2000.",
"Feed me. I'm starving.... come to think of it... i havn't eatin' since 2002.",
"Fgsfds",
"Finally I am becoming stupider no more",
"Flooring inspector.",
"Follow me to hell.",
"Follow me to hell.",
"Fool me once, shame on you. Fool me twice, ...",
"For you, scum; only 700,000,000 for this grave reserve.",
"GE8BT",
"Game over, man. Game over.",
"Ganked again...",
"George W. Bush had everything and look where he ended up. No.. not up there u idiot... down here.",
"Go Team Ant!",
"Go Team Ant!",
"Go away",
"Go away! I'm trying to take a nap in here! Bloody adventurers...",
"Go to Hell - Do not pass go, do not collect 200 zorkmids",
"God, Killed by Logic",
"Gone fishin'",
"Good night, sweet prince: And flights of angels sing thee to thy rest!",
"Greg didn't fit anywhere, accept in this grave!",
"Greg the Ghoul's All Nite Diner: All you can eat, under your feet",
"Guybrush Threepwood, Mitey Pirate",
"GwNmL",
"H is for Hector done in by a thug",
"HEEM SLEEPY",
"Hanniwa",
"Haters gonna hate",
"He died at the console, of hunger and thirst. Next day he was buried, face-down, nine-edge first.",
"He farmed his way here",
"He shouldn't have fed it.",
"He waited too long",
"Help! I'm trapped in a gravestone factory!",
"Here Lies Patient Zero",
"Here Rest The Three Stooges",
"Here lays Butch, we planted him raw, he was quick on the trigger but slow on the draw.",
"Here lies $cat killed by $playername",
"Here lies $playername. He never scored.",
"Here lies /usr/bin/nethack, killed by SIGSEGV.",
"Here lies @",
"Here lies Acid blob killed by bob",
"Here lies Balin, son of Fundin, Lord of Moria",
"Here lies Bob / decided to try an acid blob",
"Here lies Bugs Bunny. His doc couldn't figure out what was up.",
"Here lies Chester Moores, killed by a poisoned corpse. Stayed dead - his killer has no re-Moores.",
"Here lies Curious George. Reached for a candy bar, crushed by a morning star.",
"Here lies Dudley, ate a ring of death while polymorphed (ever tried?)",
"Here lies Dudley, killed by an exploding ring of full warning",
"Here lies Dudley, killed by another %&#@#& newt.",
"Here lies Dudley. Drank a potion of death (ever found one?)",
"Here lies Ed. He was a dork.",
"Here lies Edd. He was an even bigger dork.",
"Here lies Eddie. He lied, he cheated, he stole. Sadly he's dead, God bless his soul.",
"Here lies Elric killed by touching Stormbringer",
"Here lies Elric, killed by touching Stormbringer",
"Here lies Elvis. Yes, he's really dead.",
"Here lies Ezekiel. Died of cholera.",
"Here lies Fred/ Fred the dead/ Fred was in bed/ Then he ate Bread/ Now Fred is dead.",
"Here lies God, killed by Nietzsche",
"Here lies Gregg. Choked on an egg.",
"Here lies Gronkh, blasted by a Creeper",
"Here lies Henry. May he find that place called Paradise.",
"Here lies Heywood Ucuddleme, a orc who never had a hug.",
"Here lies Izchak, shopkeeper and shopmaker.",
"Here lies Jeff, he angered the shopkeeper, again",
"Here lies Joe Hacker, starved to death while playing nethack.",
"Here lies Johnny Blake/ Stepped on the gas instead of the brake.",
"Here lies Lament. Killed by nerdrage over a wand of wishing.",
"Here lies Lester Moore. 4 shots from a .44. No less... No Moore.",
"Here lies Lies. It's True.",
"Here lies Mallory Pike. She had a mean serve.",
"Here lies Nietzsche, killed by Gog",
"Here lies Original Bubs. He can see you from here.",
"Here lies Owen Moore; gone away, owin' more than he could pay.",
"Here lies Paul. You knew this was coming.",
"Here lies Poot. He is dead.",
"Here lies Sgeo, killed while reading a gravestone.",
"Here lies Steve Irwin: Rememberance for his incredible journeys.",
"Here lies Strong Bad, Checking e-mails and kicking Cheats in the hereafter,",
"Here lies The Lady's maid, died of a Vorpal Blade.",
"Here lies Tom killed by Jerry",
"Here lies Urist McDwarf. Do NOT strike the earth here, please.",
"Here lies Urist McMiner, killed by falling multiple z-levels.",
"Here lies a hacker, buried alive.",
"Here lies a happy hacker, killed by a hallucinogen-distorted succubus, while helpless.",
"Here lies a hobbit of the Shire, died of a wand of fire.",
"Here lies a jerk who forgot to pay.",
"Here lies a man who never backed down.",
"Here lies a man. His name is Robert Paulson.",
"Here lies a man. His name is Robert Paulson.",
"Here lies a programmer. Killed by a fatal error.",
"Here lies a samurai named Ken.",
"Here lies an Irishman, a good lawyer and an honest man. It's pretty crowded down there.",
"Here lies andy. Peperony and chease.",
"Here lies foo. Killed by 4chan.",
"Here lies good old Ned. A great big rock fell on his head.",
"Here lies ron3090, killed by kicking a ******* wall.",
"Here lies ron3090, killed by kicking a fucking wall.",
"Here lies scwizard, killed while tying to assent.",
"Here lies the Gnome with a Wand of Death, vanquished by a black dragon's breath. Damn RNG.",
"Here lies the body of Johann Gambolputty de von Ausfern-schplenden-schlitter-crasscrenbon --More--",
"Here lies the dumbest son-of-a-b**** ever to set foot in the Dungeons of Doom.",
"Here lies the dumbest son-of-a-bitch ever to set foot in the Dungeons of Doom.",
"Here lies the english language, killed by instant messaging",
"Here lies the left foot of Jack, killed by a land mine. Let us know if you find any more of him.",
"Here lies the man from Nantucket. He was attacked by a orc, what was left was put into a bucket.",
"Here lies the planet earth. don't dig it.",
"Here lies yetanotherhacker, killed by the wrath of RNG",
"Here lies you, in preparation.",
"Hey, get off my grave!",
"His last words? 'What throne room?'",
"Hmm... What's tha--",
"I Used Up All My Sick Days, So I Called in Dead",
"I admit no wrongdoing!",
"I ain't even mad.",
"I am not human. This mind and body are constructs. Yes, as is this sorrow.",
"I drank *what*, now?",
"I iz ded. Ded kittehs dont need bafs.",
"I need rest...",
"I never could get the hang of Thursdays.",
"I said I'm not dead yet!",
"I told him it wasn't edible...",
"I told you I was ill",
"I told you I was sick!",
"I took both pills!",
"I used to search for the Amulet of Yendor, but then I took an arrow to the knee.",
"I was thirsty.",
"I will survive!",
"I wonder what cockatrice tastes like..",
"I wonder what that little ^ sign is...",
"I wonder what this button does?",
"I wonder what this potion does...",
"I wonder what this potion does...",
"I'd rather be sailing.",
"I'll be back.",
"I'll live to die another day... or not.",
"I'm actually quite busy on the cellular level.",
"I'm gonna make it!",
"I've really been a fool",
"IWwKl",
"If a man's deeds do not outlive him, of what value is a mark in stone?",
"If only I had worn a hard hat . . .",
"If you were dead, you'd be home by now!",
"In honor of Dudley, choked to death on a ring of invisibility",
"In remembrance of Spike Milligan: He told you he was sick.",
"It looks like I'm going to have to jump...",
"It's nothing... it's nothing...",
"It's only a flesh wound; nothing serious.",
"James Brown is dead.",
"Jeanne -- She died and we did nothing.",
"JoXoD",
"JrDSt",
"Just one more hit . . .",
"Just resting a while.",
"KYNMh",
"Killed by a black dragon -- This grave is empty",
"Killed by a succubus with AIDS.",
"Killed by an improbability",
"Killed by playing SLASH'EM",
"Killed in a teleporting accident.",
"Killed over a dilithium crystal.",
"Last Words: Cheeseburger Please.",
"Last words are for wimps",
"Leeroy Jenkins - Killed by overconfidence",
"Let me out of here!",
"Life's a piece of shit / when you look at it / life's a laugh and death's a joke, it's true.",
"Lions and Tigers and Bears! Oh My!",
"Little Johnny was a chemist. Now he is no more. 'Cause what he thought was H20, was H2SO4.",
"Lm1xg",
"LnAwY",
"Lookin' good, Medusa.",
"Lorem Ipsum",
"LqAWq",
"M-M-M-M-M-MONSTER KILL!",
"MDrdP",
"MMg5P",
"Mental note: Hold sword by blunt end.",
"Mistakenly performed a sword swallowing stunt with a vorpal blade.",
"Mrs. Smith, choked on an apple. She left behind grieving husband, daughter, and granddaughter.",
"My only regret was eating my 40th pancake",
"My only regret was not having had more soda. -- Keith Forbes",
"Myrnd",
"N is for Nevill who died of ennui",
"N9MDx",
"NONE OF YOU ARE SAFE",
"NOOOOOOOOOOOOOOOOOOOOOOOOOOOO!!!!!!!!",
"Nah, that scorpion wasn't poisonous, i'm sure of it.",
"National Park. Digging is prohibited.",
"Never get involved in a land war in Asia.",
"Never go in against a Sicilian when death is on the line.",
"Newb",
"Nietzsche is dead. -- God",
"NmcOi",
"No! I don't want to see my damn conduct!",
"Nobody believed her when she said her feet were killing her",
"Not another ******* floating eye.",
"Not another fucking floating eye.",
"Nqtb1",
"OMG LAG",
"OYl1A",
"Oh Zed, Woe Zed. No where left to go Zed. All alone, and so dead. Poor Zed.",
"Oh man I am so psyched for this.",
"Oh, no. Not again.",
"Omae wa mo shindeiru!",
"On Vacation",
"On the whole, I'd rather be in Minetown.",
"One corpse, sans head.",
"Only YOU can prevent fountain overflow",
"Oooh, what's this scroll do?",
"Oops.",
"Out to Lunch",
"PLK0H",
"Paula Schultz",
"PhbRM",
"Pwnd!",
"Q57Et",
"Q8Qwi",
"QBywK",
"Quidquid Latine dictum sit, altum sonatur.",
"Quintili Vare, legiones redde!",
"R.I.P John Jacob Jingleheimer Schmidt - His name was my name too",
"R3PmA",
"RIP @",
"RIP Bort Simpesson",
"RIP Chingy, fatally killed by wasps.",
"RIP Chingy, stung by wasps.",
"RIP Guy Fieri, stung by wasps.",
"RIP Stebe Job, stung to death by wasps.",
"RIP, better you than me",
"Repeat after me: Hastur Hastur Hastur!",
"RmR0w",
"Roses are red, Violets are blue, Omae Wa Mo Shindeiru.",
"RtEjE",
"S6qzn",
"S8MXt",
"SOLD",
"Save me Jebus!",
"Segmentation fault: core dumped.",
"Shouldn't have given away my last weapon.",
"Sic Transit Gloria Mundi",
"Since the day of my birth, my death began its walk. It is walking towards me, without hurrying.",
"Since when does a failed polymorph kill you?",
"Slipped while mounting a succubus.",
"Snape, killed by a Giant Mummy. The Giant Mummy hits! You die...",
"So much time, so little to do",
"Some crazy bastard waved me over so he could hit me. Why did I go over there?",
"Someone set us up the bomb!",
"Something in my inventory could have saved me.",
"SsHwF",
"Stan's Kozy Krypts: A Place To Spend Eternity, Not A Fortune.",
"Stan's Kozy Krypts: We bury the dead, for a lot less bread.",
"Steven Paul Jobs, 1955-2011. Thanks for the future.",
"TO LET",
"TONIGHT! WE DINE! IN HELL!",
"TVTropes ruined my life",
"TXfJ9",
"Take my stuff- I don't need it anymore.",
"Taking a year dead for tax reasons.",
"Ted Danson",
"Teracotta Warrior",
"That is not dead which can eternal lie, and in successful ascensions even Death may die.",
"That is not dead which can eternal lie. I, on the other hand...",
"That is not dead which can eternal lie. And with strange aeons even death may die.",
"That zombie had armor. I wanted armor.",
"That zombie had armor. I wanted armor.",
"The cake is a lie!",
"The invisible jabberwock hits! You die...",
"The keys were right next to each other",
"The reports of my demise are completely accurate.",
"The statue got me high.",
"There goes the neighborhood.",
"This Space Reserved",
"This could by YOU!",
"This grave is reserved - for you!",
"This gravestone does not indicate a bones file.",
"This gravestone provided to you by FreeStone.",
"This headstone intentionally left blank.",
"This is a gravestone, not an altar Trololololololol",
"This is on me.",
"This man died because he sucked at nethack.",
"This space for rent; go to http://www.alt.org/nethack/addmsgs/",
"This space intentionally left blank.",
"This was a triumph.",
"This was actually just a pit, but since there was a corpse, we filled it.",
"This way to the crypt.",
"TlK5K",
"To the strongest!",
"Trolls just want to be loved!",
"Tu quoque, Brute?",
"Turns out, you really shouldn't use the phone during a thunderstorm.",
"Twilight Sparkle, killed while fighting sapient $fruit.",
"Uisne Agnoscere Mercem?",
"UsiJF",
"VACANCY",
"WHO'S THEY AND WHY DO THEY KEEP SAYING IT? - Died to bad advice.",
"Wait for that wisest of all counselors, Time",
"We introduced Eg to the giant, but he misunderstood.",
"We wonder what kil",
"Welcome!",
"What could possibly go wrong?",
"What happen when I press the button?",
"What the hell is genocidal confusion?",
"When you find yourself in a hole, stop digging.",
"When you zap a wand of death, make sure you're pointing it the right way.",
"Where were you Kos?",
"Wheres the beef?!",
"While we are postponing, life speeds by.",
"Whoops.",
"Whoops....",
"Why, oh why didn't I take the BLUE pill?",
"Wipe your feet before entering",
"Wish me best of luck in my future endeavors....",
"Wish you were here!",
"Worse things happen at sea, you know.",
"WqLjM",
"X66P9",
"XXxyZ",
"Y is for Yorick whose head was knocked in",
"Y.A.S.D.",
"YAAD",
"YASD",
"YASD'd!",
"Yea, it got me too.",
"You asked me if it would kill me to be civil...well, now you know.",
"You come from nothing, you're going back to nothing. What have you lost? Nothing!",
"You die... --More--",
"You should see the other guy.",
"You're never too dead to learn.",
"You're standing on my head.",
"Your very silence shows you agree",
"Zed's dead, baby.",
"Zed's dead, baby. Zed's dead.",
"addFU",
"asasa",
"asdg",
"brb",
"bvOwX",
"cAWZB",
"cGf2N",
"cg5Vx",
"confusion will be my epitaph",
"cxjUZ",
"cy4Wa",
"doQmo",
"dxekm",
"edw",
"eee3e",
"enPWu",
"fW6Ye",
"fj2In",
"fopBQ",
"g3cwM",
"g5NJc",
"gIWDx",
"gwNNZ",
"hVhfq",
"here lies Krass the cheapskate... still owes me 10 Gold for engraving this gravestone...",
"here lies andy -- peperoni and cheese",
"hukQY",
"j16IV",
"jVHLs",
"jeBrG",
"kGIOZ",
"lgALT",
"logout;",
"lol",
"lzBU6",
"m4xlc",
"mde.tv",
"mkelO",
"mv170",
"mxQ0M",
"n8jyF",
"never try to kill a shopkeeper who has a wand of sleep, it'll be the last thing you do.",
"njnm",
"oPbQR",
"oPlHb",
"ojN8O",
"on7ZP",
"or2SE",
"p0RFR",
"pWRbl",
"pz6Yk",
"q4JhO",
"qJOkM",
"qnnZA",
"quit, on",
"rACim",
"rest in peace dudley killed by a newt AGAIN!!!!",
"rlDrk",
"romanes eunt domus",
"ruNc2",
"sX2l3",
"sbpzV",
"slXdb",
"swyyI",
"test",
"test test",
"tlbto",
"tm5QP",
"try #quit",
"u5fPY",
"uE54o",
"uWD2q",
"uoZXt",
"wK3qm",
"wKJBc",
"wKiLp",
"wc8Zl",
"wzURY",
"x982a",
"xS3K6",
"xdwdwewewrwerfew",
"xyO0Y",
"yY5OU",
"zKKBC",
"zZIk8",
"zmtgs",
	"Alas fair Death, 'twas missed in life - some peace and quiet from my wife",
	"Applaud, my friends, the comedy is finished.",
	"At last... a nice long sleep.",
	"Audi Partem Alteram",
	"Basil, assaulted by bears",
	"Burninated",
	"Confusion will be my epitaph",
	"Do not open until Christmas",
	"Don't be daft, they couldn't hit an elephant at this dist-",
	"Don't forget to stop and smell the roses",
	"Don't let this happen to you!",
	"Dulce et decorum est pro patria mori",
	"Et in Arcadia ego",
	"Fatty and skinny went to bed.  Fatty rolled over and skinny was dead.  Skinny Smith 1983-2000.",
	"Finally I am becoming stupider no more",
	"Follow me to hell",
	"...for famous men have the whole earth as their memorial",
	"Game over, man.  Game over.",
	"Go away!  I'm trying to take a nap in here!  Bloody adventurers...",
	"Gone fishin'",
	"Good night, sweet prince: And flights of angels sing thee to thy rest!",
	"Go Team Ant!",
	"He farmed his way here",
	"Here lies Lies. It's True",
	"Here lies the left foot of Jack, killed by a land mine.  Let us know if you find any more of him",
	"He waited too long",
	"I'd rather be sailing",
	"If a man's deeds do not outlive him, of what value is a mark in stone?",
	"I'm gonna make it!",
	"I took both pills!",
	"I will survive!",
	"Killed by a black dragon -- This grave is empty",
	"Let me out of here!",
	"Lookin' good, Medusa.",
	"Mrs. Smith, choked on an apple.  She left behind grieving husband, daughter, and granddaughter.",
	"Nobody believed her when she said her feet were killing her",
	"No!  I don't want to see my damn conduct!",
	"One corpse, sans head",
	"On the whole, I'd rather be in Minetown",
	"On vacation",
	"Oops.",
	"Out to Lunch",
	"SOLD",
	"Someone set us up the bomb!",
	"Take my stuff, I don't need it anymore",
	"Taking a year dead for tax reasons",
	"The reports of my demise are completely accurate",
	"(This space for sale)",
	"This was actually just a pit, but since there was a corpse, we filled it",
	"This way to the crypt",
	"Tu quoque, Brute?",
	"VACANCY",
	"Welcome!",
	"Wish you were here!",
	"Yea, it got me too",
	"You should see the other guy",
	"...and they made me engrave my own headstone too!",
	"<Expletive Deleted>",
	"Adapt. Enjoy. Survive.",
	"Adventure, hah! Excitement, hah!",
	"After all, what are friends for...",
	"After this, nothing will shock me",
	"Age and treachery will always overcome youth and skill",
	"Ageing is not so bad.  The real killer is when you stop.",
	"Ain't I a stinker?",
	"Algernon",
	"All else failed...",
	"All hail RNG",
	"All right, we'll call it a draw!",
	"All's well that end well",
	"Alone at last!",
	"Always attack a floating eye from behind!",
	"Am I having fun yet?",
	"And I can still crawl, I'm not dead yet!",
	"And all I wanted was a free lunch",
	"And all of the signs were right there on your face",
	"And don't give me that innocent look either!",
	"And everyone died.  Boo hoo hoo.",
	"And here I go again...",
	"And nobody cares until somebody famous dies...",
	"And so it ends?",
	"And so... it begins.",
	"And sometimes the bear eats you.",
	"And then 'e nailed me 'ead to the floor!",
	"And they said it couldn't be done!",
	"And what do I look like?  The living?",
	"And yes, it was ALL his fault!",
	"And you said it was pretty here...",
	"Another lost soul",
	"Any day above ground is a good day!",
	"Any more of this and I'll die of a stroke before I'm 30.",
	"Anybody seen my head?",
	"Anyone for deathmatch?",
	"Anything for a change.",
	"Anything that kills you makes you ... well, dead",
	"Anything worth doing is worth overdoing.",
	"Are unicorns supposedly peaceful if you're a virgin?  Hah!",
	"Are we all being disintegrated, or is it just me?",
	"At least I'm good at something",
	"Attempted suicide",
	"Auri sacra fames",
	"Auribus teneo lupum",
	"Be prepared",
	"Beauty survives",
	"Been Here. Now Gone. Had a Good Time.",
	"Been through Hell, eh? What did you bring me?",
	"Beg your pardon, didn't recognize you, I've changed a lot.",
	"Being dead builds character",
	"Beloved daughter, a treasure, buried here.",
	"Best friends come and go...  Mine just die.",
	"Better be dead than a fat slave",
	"Better luck next time",
	"Beware the ...",
	"Bloody Hell...",
	"Bloody barbarians!",
	"Buried the cat.  Took an hour.  Damn thing kept fighting.",
	"But I disarmed the trap!",
	"Can YOU fly?",
	"Can you believe that thing is STILL moving?",
	"Can you come up with some better ending for this?",
	"Can you feel anything when I do this?",
	"Can you give me mouth to mouth, you just took my breath away.",
	"Can't I just have a LITTLE peril?",
	"Can't eat, can't sleep, had to bury the husband here.",
	"Can't you hit me?!",
	"Chaos, panic and disorder.  My work here is done.",
	"Check enclosed.",
	"Check this out!  It's my brain!",
	"Chivalry is only reasonably dead",
	"Coffin for sale.  Lifetime guarantee.",
	"Come Monday, I'll be all right.",
	"Come and see the violence inherent in the system",
	"Come back here!  I'll bite your bloody knees off!",
	"Commodore Business Machines, Inc.   Died for our sins.",
	"Complain to one who can help you",
	"Confess my sins to god?  Which one?",
	"Confusion will be my epitaph",
	"Cooties?  Ain't no cooties on me!",
	"Could somebody get this noose off me?",
	"Could you check again?  My name MUST be there.",
	"Could you please take a breath mint?",
	"Couldn't I be sedated for this?",
	"Courage is looking at your setbacks with serenity",
	"Cover me, I'm going in!",
	"Crash course in brain surgery",
	"Cross my fingers for me.",
	"Curse god and die",
	"Dead Again?  Pardon me for not getting it right the first time!",
	"Dead and loving every moment!",
	"Dear wife of mine. Died of a broken heart, after I took it out of her.",
	"Don't tread on me!",
	"Dragon? What dragon?",
	"Drawn and quartered",
	"Either I'm dead or my watch has stopped.",
	"Eliza -- Was I really alive, or did I just think I was?",
	"Elvis",
	"Enter not into the path of the wicked",
	"Eris?  I don't need Eris",
	"Eternal Damnation, Come and stay a long while!",
	"Even The Dead pay taxes (and they aren't Grateful).",
	"Even a tomb stone will say good things when you're down!",
	"Ever notice that live is evil backwards?",
	"Every day is starting to look like Monday",
	"Every day, in every way, I am getting better and better.",
	"Every survival kit should include a sense of humor",
	"Evil I did dwell;  lewd did I live",
	"Ex post fucto",
	"Excellent day to have a rotten day.",
	"Excuse me for not standing up.",
	"Experience isn't everything. First, You've got to survive",
	"First shalt thou pull out the Holy Pin",
	"For a Breath, I Tarry...",
	"For recreational use only.",
	"For sale: One soul, slightly used. Asking for 3 wishes.",
	"For some moments in life, there are no words.",
	"Forget Disney World, I'm going to Hell!",
	"Forget about the dog, Beware of my wife.",
	"Funeral - Real fun.",
	"Gawd, it's depressing in here, isn't it?",
	"Genuine Exploding Gravestone.  (c)Acme Gravestones Inc.",
	"Get back here!  I'm not finished yet...",
	"Go ahead, I dare you to!",
	"Go ahead, it's either you or him.",
	"Goldilocks -- This casket is just right",
	"Gone But Not Forgotten",
	"Gone Underground For Good",
	"Gone away owin' more than he could pay.",
	"Gone, but not forgiven",
	"Got a life. Didn't know what to do with it.",
	"Grave?  But I was cremated!",
	"Greetings from Hell - Wish you were here.",
	"HELP! It's dark in here... Oh, my eyes are closed - sorry",
	"Ha! I NEVER pay income tax!",
	"Have you come to raise the dead?",
	"Having a good time can be deadly.",
	"Having a great time. Where am I exactly??",
	"He died of the flux.",
	"He died today... May we rest in peace!",
	"He got the upside, I got the downside.",
	"He lost his face when he was beheaded.",
	"He missed me first.",
	"He's not dead, he just smells that way.",
	"Help! I've fallen and I can't get up!",
	"Help, I can't wake up!",
	"Here lies Pinocchio",
	"Here lies the body of John Round. Lost at sea and never found.",
	"Here there be dragons",
	"Hey, I didn't write this stuff!",
	"Hodie mihi, cras tibi",
	"Hold my calls",
	"Home Sweet Hell",
	"I KNEW this would happen if I lived long enough.",
	"I TOLD you I was sick!",
	"I ain't broke but I am badly bent.",
	"I ain't old. I'm chronologically advantaged.",
	"I am NOT a vampire. I just like to bite..nibble, really!",
	"I am here. Wish you were fine.",
	"I am not dead yet, but watch for further reports.",
	"I believe them bones are me.",
	"I broke his brain.",
	"I can feel it.  My mind.  It's going.  I can feel it.",
	"I can't go to Hell. They're afraid I'm gonna take over!",
	"I can't go to hell, they don't want me.",
	"I didn't believe in reincarnation the last time, either.",
	"I didn't mean it when I said 'Bite me'",
	"I died laughing",
	"I disbelieved in reincarnation in my last life, too.",
	"I hacked myself to death",
	"I have all the time in the world",
	"I knew I'd find a use for this gravestone!",
	"I know my mind. And it's around here someplace.",
	"I lied!  I'll never be alright!",
	"I like it better in the dark.",
	"I like to be here when I can.",
	"I may rise but I refuse to shine.",
	"I never get any either.",
	"I said hit HIM with the fireball, not me!",
	"I told you I would never say goodbye.",
	"I used to be amusing. Now I'm just disgusting.",
	"I used up all my sick days, so now I'm calling in dead.",
	"I was killed by <illegible scrawl>",
	"I was somebody. Who, is no business of yours.",
	"I will not go quietly.",
	"I'd give you a piece of my mind... but I can't find it.",
	"I'd rather be breathing",
	"I'll be back!",
	"I'll be mellow when I'm dead. For now, let's PARTY!",
	"I'm doing this only for tax purposes.",
	"I'm not afraid of Death!  What's he gonna do? Kill me?",
	"I'm not getting enough money, so I'm not going to engrave anything useful here.",
	"I'm not saying anything.",
	"I'm weeth stupeed --->",
	"If you thought you had problems...",
	"Ignorance kills daily.",
	"Ignore me... I'm just here for my looks!",
	"Ilene Toofar -- Fell off a cliff",
	"Is that all?",
	"Is there life before Death?",
	"Is this a joke, or a grave matter?",
	"It happens sometimes. People just explode.",
	"It must be Thursday. I never could get the hang of Thursdays.",
	"It wasn't a fair fight",
	"It wasn't so easy.",
	"It's Loot, Pillage and THEN Burn...",
	"Just doing my job here",
	"Killed by diarrhea of mouth and constipation of brain.",
	"Let her RIP",
	"Let it be; I am dead.",
	"Let's play Hide the Corpse",
	"Life is NOT a dream",
	"Madge Ination -- It wasn't all in my head",
	"Meet me in Heaven",
	"Move on, there's nothing to see here.",
	"My heart is not in this",
	"No one ever died from it",
	"No, you want room 12A, next door.",
	"Nope.  No trap on that chest.  I swear.",
	"Not again!",
	"Not every soil can bear all things",
	"Now I have a life",
	"Now I lay thee down to sleep... wanna join me?",
	"Obesa Cantavit",
	"Oh! An untimely death.",
	"Oh, by the way, how was my funeral?",
	"Oh, honey..I missed you! She said, and fired again.",
	"Ok, so the light does go off. Now let me out of here.",
	"One stone brain",
	"Ooh! Somebody STOP me!",
	"Oops!",
	"Out for the night.  Leave a message.",
	"Ow!  Do that again!",
	"Pardon my dust.",
	"Part of me still works.",
	"Please, not in front of those orcs!",
	"Prepare to meet me in Heaven",
	"R2D2 -- Rest, Tin Piece",
	"Relax.  Nothing ever happens on the first level.",
	"Res omnia mea culpa est",
	"Rest In Pieces",
	"Rest, rest, perturbed spirit.",
	"Rip Torn",
	"She always said her feet were killing her but nobody believed her.",
	"She died of a chest cold.",
	"So let it be written, so let it be done!",
	"So then I says, How do I know you're the real angel of death?",
	"Some patients insist on dying.",
	"Some people have it dead easy, don't they?",
	"Some things are better left buried.",
	"Sure, trust me, I'm a lawyer...",
	"Thank God I wore my corset, because I think my sides have split.",
	"That is all",
	"The Gods DO have a sense of humor: I'm living proof!",
	"This dungeon is a pushover",
	"This elevator doesn't go to Heaven",
	"This gravestone is shareware. To register, please send me 10 zorkmids",
	"This gravestone provided by The Yendorian Grave Services Inc.",
	"This is not an important part of my life.",
	"This one's on me.",
	"This side up",
	"Tone it down a bit, I'm trying to get some rest here.",
	"Virtually Alive",
	"We Will Meet Again.",
	"Weep not, he is at rest",
	"Welcome to Dante's.  What level please?",
	"Well, at least they listened to my sermon...",
	"Went to be an angel.",
	"What are you doing over there?",
	"What are you smiling at?",
	"What can you say, Death's got appeal...!",
	"What pit?",
	"When the gods want to punish you, they answer your prayers.",
	"Where's my refund?",
	"Will let you know for sure in a day or two...",
	"Wizards are wimps",
	"Worms at work, do not disturb!",
	"Would you mind moving a bit?  I'm short of breath down here.",
	"Would you quit being evil over my shoulder?",
	"Yes Dear, just a few more minutes...",
	"You said it wasn't poisonous!",

	/* from http://www.alt.org/nethack/addmsgs/viewmsgs.php */
	"Balin, son of Fundin, Lord of Moria",
	"Memento mori",
	"Help! I'm trapped in a gravestone factory!",
	"This grave is reserved - for you!",
	"This gravestone does not indicate a bones file.",
	"This space for rent; go to http://www.alt.org/nethack/addmsgs/"
	"Here lies Sgeo, killed while reading a gravestone.",
	"Quidquid Latine dictum sit, altum videtur.",
	"Death is but a door. Time is but a window. I'll be back!",
	"The reports of my demise are completely accurate.",
	"Bulk deals on gravestones - Die today!",

	/* from UnNetHack */
	"Hack 1984-1985",
	"NetHack 1987-2003",

	/* from UnNetHackPlus */
	"SporkHack 2007-2010",
	"SLASH'EM 1997-2006",
	"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.",
	"Lost to the guy who is passway!",
	"Went through cave just now. Still spiritful.",
	"Tie with rope!",
	"Finally it will be return to the center!",
	"I will not suspend it.",
	"But the bag is full!",
	"We are defeated by Shangdao team!",
	"I will try best not to incommode. Everybody aftertime",
	"The wave is small, may is urf?",
	"Great waterfall! Can I drift?",
      "OH! OH! Draw it! Put it down it is excessively too!!",
	"Put it down what to do? Oh the badge... sorry for having forgotten it want to get a regularity badge.",
	"Give it to you after a while!",
	"There is some dimness inside. The skill is to experience the charm of pocket monster. Is it fit to me?",
	"I defeated her. I try my best, but still not strong and no confidence. If lost out try again nothing at all!",
	"I will not equest sister. It is good in this status, dear!",
	"I go to Xiaojin specially the bike isn't really solded, isn't it?",
	"It is safe for man too!",
	"It always lost in the game of monster. Always let it die because of it isn't closed with me",
	"Nobody is care of it/it's impossible/dislike to go out to play/it's good to play together",
	"Once, my husband has done nothing. Only fond of entertainment how can I do?",
	"I didn't call him, he had gone to game-shop.",
	"My husband is regarded as a monster cracy Zhenqian is like to his father!",
	"Oh!Loged on such at most!",
	"The trolley isn't in, it's impossible to carry the guest on my back to walk!",
"Once I walked in darkness I saw the moving-tree.",
"My sister felt funny and go to see it, I afraid that it was dangerous!",
"I told my elder sister about the moving-tree, she said it was dangerous, don't let me go unless I defeated Miss Yu.",
"Today many goods to be bought!",
"I wondered whose tel no. to be longed no?",
"I feel Xiaojin is in a big city while going to its store.",
"Buy a gift for my friends go to the second floor:Market!",
"Isn't it improved the gear while playing?",
"The feeling is very good when the special ability is improved!",
"A magical gift to send gift!",
"Elder sister will come on sunday to look the state of monster, take the skill machine!",
"The monster taken from others whose name isn't altered, because the name includes the feeling of a great man!",
"Tired while shopping. Go to the top floor to have a rest.",
"There is a thing I want very much, but the money isn't enough!",
"Look the distant with telescope. Our house maybe seen.Is it the green roof?",
"But the percentage of lose is low in fact, I like the card machine.",
"Losed to the coin machine losed to the card machine too!",
"It's regretted very much, don't find the object for exchange take the monster being kept!",
"No prop but mail!",
"It is a regret cannn't found the object for exchanging because of keeping for a long time!",
"Before contacting with the center, wrote a report.",
"This is a mystic egg be careful, please!",
"Its content is your note about exploration,and character synopsis. The telephone hasn't sent.",
"What a big monster's center! Very good, haveing so many new machines! As if it has been set up just!",
"An unknown girl gave me a cat just now. Do you believe that it will be fine when exchange with?",
"I was afraided when found my friend's name merge in the news!",
"After the that we can connect with various of people certainly.",
"The facility there can't be used now. It will feel good to go to the hearsaid place earlier!",
"Becoming famous, after the name merged in the news. All right, how to make one's name merge in the news?",
"It is excited to communicate! With this means first?",
"When exchanging at the exchange department, and seeing the news about monsters.",
"The main comes to manage center. Look out everybody, now the monster coachs around the countryuse the electric wave to communicate.",
"The forest looks strange. It isn't good to walk now.",
"There is sweet taste inside! The monster seems to go out.",
"Shangdao themselves look down on us, how to stand it!",
"Drive in the bikeway without hands the feeling is good!",
"Ssee the relic, there is a sense suddenly!",
"I think there is a great secret hidden in the relic!",
"The shaking just frightens me!But we are care about the wall very much.",
"Did the monster appear in the relic!? This is a great invent! We start to investigate it at once!",
"How many sort monsters are there in the relic?",
"The riddle's appearance has some causes certainly. Now we are invertigating this matter.",
"My result is the riddle's appearance appeared after the communion center had been built.",
"Clear up the rogatory content it is going to be published. If so,I'll be as famous as Wuzhimu Dr.",
"I am living there if you like,you may come to reside. There are many empty places.",
"Every week one monster always comes to waterside. Want to see the monster very much!",
"I droped from the rope and brode my waist, I cann't move.",
"Do well, the Missile Bombs escapes. My waist becomes well! We arw going to go back!",
"Here lies the Hunger Games trilogy. Slain by the romantic plot tumor named Peeta.",
"Here lies Bella Swan. Killed by her abusive bastard boyfriend Edward.",
"Here lies Jacob Black. He killed himself after he realized Edward would get Bella even though Jacob was a far better partner for her in every way.",
"Here lies Neville Longbottom. Guess Hermione finally stopped missing him with her Avada Kedavra curse.",
"Here lies Dolores Umbridge. Shot in the head by Hermione's Avada Kedavra death ray. Yep, Rowling - your characters should stop acting stupid and kill Umbridge already!",
"Here lies Lord Voldemort. Killed by his own reflecting death ray.",
"Here lies Bellatrix Lestrange. She took a knife to the head.",
"Here lies Peeta Mellark. He took a knife to the head and we're not as unrealistic as his 'author'. He shouldn't even have existed anymore in books 2 and 3!",
"Here lies Cato. Killed by an unrealistic author who didn't realize he and Katniss were the one true pairing.",
"Here lies Gale. He choked himself to death on a rope when he realized that Peeta, that asshole, would put his dick in Katniss' cunt.",
"Here lies Ron Weasley. Killed by brainlessness.",
"Here lies Vincent Crabbe. Killed by a demonic fire.",
"Here lies Dobby the elf. He died totally in vain, and because his author was on a killing spree to get rid of 'unneccessary' characters.",
"Here lies Cedric Diggor-erm, Edward Cullen. Killed by an overdose of sparkle.",
"Here lies Edward Cullen. Killed by an angry mob for being an abusive son-of-a-bitch.",
"Here lies Christian Gray. Anastasia finally started to fight him and whipped him to death with his own belt.",
"Here lies Sirius Black. Cause of death: being too stupid to simply use the Avada Kedavra curse on Bellatrix.",
"Here lies Vernon Dursley. He fell out of his window and broke his neck. Really, Rowling, why the heck would you let him live??? He fucking died! Dammit!",
"Here lies Dolores Umbridge. She was raped and successively killed by Bane the centaur. NO, Rowling, one does NOT return from them alive. Just NO.",
"Here lies Albus Dumbledore. Killed by a cold-blooded murderer named Snape.",
"Here lies Severus Snape. One of many, many people in the Harry Potter series who died just because their author wanted them to die.",
"Here lies the main character of Rowling's 'adult book'. Killed by cutting herself. Seriously, Rowling, why do you write such crap anyway???",
"Here lies Alastor Mad-Eye Moody. He would have lived if it weren't for his sadistic author who wanted to 'shock' the readers by presenting lots of corpses.",
"Here lies Nymphadora Tonks. Well, nobody cared about that pseudo-Mary Sue anyway. I'd much rather read stories about Ebony Dark'ness.",
"Here lies Rita Skeeter. Killed by her own quill which decided it had enough of her using it to write filthy lies.",
"Here lies Draco Malfoy. Killed by Harry Potter with the Sectumsempra curse. Really now, there's no reason why Harry would stop using it prior to killing Draco.",
"Here lies Big Bad Pete. Bludgeoned by Minnie Mouse with a high-heeled lady's shoe.",
"Here lies Donald Duck. He foolishly allowed Daisy to see him with another woman and was too stupid to run away.",
"Live and let live, right..?",
"AAAAAAAAARRRRRRRRRRRRRRGGGGGGGGGGGGGHHHHHHHHHHHHHHH!!!!!!!!!!!!",
"AAAARRRGGGHHH!!!",
"Somehow, I have a bad feeling about this...",
"Strangely, all of a sudden I don't feel so good.",
"You can see armored women on winged horses coming for you.",
"Oh well, you can't always win.",
"I'm too young to die!",
"I'll be back!",
"O, untimely death!",
"Slave, thou hast slain me!",
"Ouch! That smarts!",
"Who knocked?",
"Did anybody get the number of that truck..?",
"Ouch.",
"Et tu, Brute! Then fall, Caesar!",
"O! I die, Horatio...",
"I told you to be careful with that sword...",
"This guy's a little crazy...",
"Ok, ok, I get it: No more pals.",
"No more mr. nice guy!",
"Who turned off the light..?",
"Join the army, see the world, they said...",
"Mom told me there'd be days like this...",
"Rats!",
"Shall this fellow live?",
"Help, ho!",
"What ho! Help!",
"What hast thou done?",
"I'll be revenged on the whole pack of you!",
"You will *pay* for this!",
"They say blood will have blood...",
"Violence is no solution!",
"Yes?",
"#&%#&#%*#*&%!!!!!",
"F***!",
"No time to make a testament?",
"Ugh!",
"Aargh!",
"Aaagghhh!",
"I'm melting!",
"Oof..",
"Oh!",
"Did somebody knock?",
"Later, dude...",
"CU!",
"What? Who? Me? Oh, s..t!",
"...amen!",
"Eeek!",
"Aacch!",
"I hate it when that happens.",
"One direct hit can ruin your whole day.",
"Oh no!",
"Not me!",
"Ouch.",
"Oh no, not again.",
"Another one bites the dust.",
"Goodbye.",
"Help me!",
"Farewell, cruel world.",
"Oh man!",
"Doough!",
"This is the End, my only friend.",
"It's all over.",
"The fat lady sang.",
"Why does everything happen to me?",
"I'm going down.",
"Crapola.",
"Pow!",
"Bif!",
"Bam!",
"Zonk!",
"I should've listened to my mother...",
"No... a Bud light!",
"What was that noise?",
"Mama said there'd be days like this.",
"It's just one of those days...",
"I see a bright light...",
"Mommy? Is that you?",
"I let you hit me!",
"Sucker shot!",
"I didn't want to live anyway.",
"-<sob>-",
"Hah haa! Missed me! Ha---",
"Was that as close as I think it was?",
"Monsters rejoice: the hero has been defeated.",
"It wasn't just a job it was an adventure!",
"I didn't like violence anyway!",
"I thought you liked me?",
"Such senseless violence! I don't understand it.",
"I think this guy's a little crazy.",
"Somehow I don't feel like killing anymore.",
"Help me! I am undone!",
"Hey! Killin' ain't cool.",
"This fell sergeant, Death, is strict in his arrest...",
"The rest is silence.",
"Guh!",
"It's game over, man!",
"You've run out of life.",
"Thou art slain.",
"Finish him!",
"Trust me, I know what I'm doing...",
"Die, mortal!",
"Kill men i' the dark! What be these bloody thieves?",
"Ho! Murder! Murder!",
"O! I am spoil'd, undone by villains!",
"O murderous slave! O villain!",
"O, falsely, falsely murder'd!",
"A guiltless death I die.",
"AAAAAAAAAAAAAAAAAAAAAAAAHHHHHHH!",
"Trust me.",
"Dammit, this thing won't die!",
"He hit me for HOW MUCH?????",
"Look, behind you!!!",
"Who fed steroids to that kobold?",
"Don't worry, be happy!",
"I don't believe this!",
"Oops.",
"Oups.",
"Can't you take a joke?",
"Well, I didn't much like this character, anyway...",
"Oops, sorry... didn't mean to disturb you.",
"I never get to have any fun!",
"Stop!",
"Cut it out!",
"Don't worry. I've got a plan.",
"It didn't look so tough.",
"Run away!",
"All clear, guys.",
"AGAIN!?!?!",
"I don't like this dungeon...",
"Maybe this wasn't such a good idea.",
"My God will protect me.",
"You wouldn't dare!",
"But what about my Parry Skill? Tumbling?",
"Don't worry - I have Pilot-7.",
"And I've *never* done you any harm.",
"I don't understand. It should be dead by now.",
"I'm heir to the crown. They wouldn't dare!",
"Hey! Where's my stomach? My hands?",
"Ha! That's the oldest trick in the book.",
"Cover me.",
"Watch this.",
"And damn'd be him that first cries, 'Hold, enough!'",
"I will not yield.",
"...but like a man he died.",
"If you cut me down, I will only become more powerful.",
"Well, at least I tried...?",
"What could possibly have gone wrong?",
"You die...",
"What's with that weirdo with the teeth?",
"Surrender? Never!",
"I'm sure reinforcements will get here on time. They promised.",
"Funny, didn't *look* like a cyberpsycho....",
"I have a very bad feeling about this.",
"Do something, SCHMUCK!",
"I feel I could cast 'Speak with Dead' and talk to myself.",
"Oh, that's just a light wound.",
"Ach, is doch nur 'ne Fleischwunde...",
"I thought you were on MY side...",
"Next time, try talking!",
"Oh shit... I'll try to teleport again.",
"Somebody get me a Rod of Resurrection... QUICK!",
"Uhh... oh-oh...",
"Gee, where'd everybody go?",
"I see it coming...aaargllhhhh! {sough}",
"What do you mean 'aaargllhhhh'? Hey man, I've paid for this.",
"Ay! Ay! Ay!",
"Ohe! Ohe! Ohe!",
"Et tu, Caesar! Then fall, Brute!",
"Even the best laid plans...",
"Hey, not too rough!",
"The Random Number Generator hates me!",
"So when I die, the first thing I will see in heaven is a score list?",
"Can't we talk this thing over?",
"Wait! Spare me and I'll make you rich! Money is not a problem!",
"I hate you!",
"By the kind gods, 'twas most ignobly done!",
"Mein Leben!",
"Meine Lieder!",
"I'm the hero of this story! I CAN'T die!",
"I thought heroes were supposed to win!",
"Gee... thanks.",
"You've fallen and can't get up!",
"911?",
"Sure don't look good...",
"Oh No! Here I blow again!",
"Hey - I've got lawyers.",
"Thanks, I needed that.",
"I AM toast!!",
"Scheisse!",
"Fatality!",
"Brutality!",
"Toasty!",
"And you thought Tristan was unlucky...",
"Just wait till I get my hands on the crook who sold me this crappy armor...",
"All is lost. Monks, monks, monks!",
"All my possessions for a moment of time!",
"Don't let poor Nelly starve!",
"Wally, what is this? It is death, my boy: they have deceived me.",
"Everyone dances with the Grim Reaper.",
"Adios.",
"I'm going home, babe.",
"I am innocent, innocent, innocent!",
"Watch where you're pointing with that sword! You nearly...",
"Hmm, some things are better wanted than had...",
"And they told me it was not loaded.",
"Of course I know what I am doing.",
"It looked harmless.",
"Hilfe, hilfe, hilfe!",
"Look, dad! No head!",
"Look! I'm flying!",
"Think I'm gonna fall for that?",
"I'll be back... as soon as I can.",
"3... 2... 1... Liftoff!",
"My wallet? In your dreams!",
"Yes! Yes! YES! YES! YY... AAARRRGGGHH!",
"See you later, alligator!",
"Up, up and awaaaayyy!",
"Been nice knowing you.",
"But I just got a little prick!",
"And I just wanted that fancy suit of armour you were carrying...",
"Hey guys, where are you?",
"Hey look... ARCHERS!",
"I can't probably miss...",
"I don't care. I have a Scroll of Raise Dead.",
"I don't care. I have a Ring of Regeneration.",
"I have this dungeon at home, I know where everything is!",
"This HAS to be an illusion. I attempt to disbelieve it.",
"I thought you could be trusted.",
"Never try to sneak in a plate mail.",
"I'll never surrender.",
"I'll use the Cheat Death option...",
"I'm invincible!",
"I'm death incarnate! Nothing can harm me!",
"Hey, it was only a joke, all right?",
"Hey, don't talk to me like that!",
"I have rights, too!",
"Just because you're big and ugly doesn't mean you can push ME around.",
"Me first! Me first!",
"Let me handle this.",
"No problem. That's easy.",
"Oh, shit.",
"So what?",
"Tell me this is an illusion... please!",
"I hate the RNG...",
"They need a twenty to hit me! I'm invincible!",
"Trust me.",
"CHARGE!",
"What do you mean, how many hit points do I have?",
"What do you mean, my GOI expired?",
"Yeah, I knew it was dangerous, but I was thinking about the experience points.",
"You mean you get to use the critical hit chart too?",
"You'd have to be a GOD to smile after that hit!",
"I'm not afraid of death. I just don't want to be there when it happens.",
"I have such sweet thoughts.",
"I pray you all pray for me.",
"I shall hear in heaven.",
"Is not this dying with courage and true greatness?",
"I must sleep now.",
"Nurse, nurse, what murder! What blood! I have done wrong!",
"It is finished.",
"That unworthy hand! That unworthy hand!",
"I am dying.",
"Oh, dear.",
"I will not kneel. Strike!",
"I have led a happy life.",
"Dying, dying.",
"I feel the flowers growing over me.",
"Now it is come.",
"Let me die to the sound of sweet music.",
"I will now enter the Halls of Mandos.",
"Ungrateful traitors!",
"We perish, we disappear, but the march of time goes on forever.",
"Youth, I forgive thee.",
"Treason! Treason!",
"Coward! Why did you not protect me?",
"I am absolutely undone.",
"It is well. I die hard, but am not afraid to go.",
"Do let me die in peace.",
"Nothing is real but pain now.",
"Violent use brings violent plans.",
"Soldier boy, made of clay, now an empty shell.",
"Bodies fill the fields I see, the slaughter never ends.",
"Life planned out before my birth, nothing could I say.",
"Blood will follow blood, dying time is here.",
"Never happy endings on these dark sets.",
"No one to play soldier now, no one to pretend.",
"Time for lust, time for lie, time to kiss your life goodbye.",
"Greetings, Death, he's yours to take away.",
"I was born for dying.",
"The higher you walk, the farther you fall.",
"Where's your crown, King Nothing?",
"Exit: light - enter: night!",
"New blood joins this earth...",
"You labeled me, I'll label you, so I dub thee unforgiven.",
"If you're gonna die, die with your boots on!",
"There's a time to live, and a time to die, when it's time to meet the maker.",
"Isn't it strange, as soon as you're born you're dying?",
"Only the good die young, all the evil seem to live forever.",
"I don't wanna die, I'm a god, why can't I live on?",
"And in my last hour, I'm a slave to the power of death.",
"Now I am cold, but a ghost lives in my veins.",
"You got to watch them - be quick or be dead.",
"Heaven can wait 'till another day.",
"You'll take my life but I'll take yours too.",
"We won't live to fight another day.",
"As I lay forgotten and alone, without fear I draw my parting groan.",
"Somebody please tell me that I'm dreaming!",
"Can't it be there's been some sort of error?",
"Is it really the end not some crazy dream?",
"Life down there is just a strange illusion.",
"Your body tries to leave your soul.",
"I'm so tired of living, I might as well end today.",
"Life, life! Death, death! How curious it is!",
"Catch my soul 'cos it's willing to fly away!",
"Flames? Not yet, I think.",
"Someone call the Gendarmes!",
"I split my brain, melt through the floor.",
"And now the dreams end.",
"Off to Never-Never Land!",
"Death greets me warm, now I will just say goodbye.",
"What is this? I've been stricken by fate!",
"This can't be happening to me!",
"Flash before my eyes: now it's time to die.",
"You have been dying since the day you were born.",
"No point asking who's to blame.",
"But for all his power he couldn't foresee his own demise.",
"My creator will lay my soul to rest.",
"Was that worth dying for?",
"Can you say you are proud of what you've done?",
"But there are some things which cannot be excused.",
"Why is it some of us are here just so that we'll die?",
"The shortest straw, pulled for you.",
"There's got to be just more to it that this or tell me why do we exist?",
"I can't believe that really my time has come.",
"Too much of a good thing, I guess...",
"I really screwed up this time.",
"Wow, what a trip!",
"What is Time, friend or foe",
"Time waits for none",
"Running through your fingers like sand",
"Taking us along to future unknown",
"And all too sudden, like it or not",
"We become part of the Land.",
"Hah! I'm not dead yet.  I still have five hit points.",
"I don't understand.  It should be dead by now.",
"I rolled a 20.  How could that be a miss?",
"What the frell?!",
"Tangado a chadad!",
"Prepare to fire!",
"Hello again, poor Yorick.",
"Ok, ok, I get it.",
"YASD",
"This one's for the YASD hall of fame.",
"I must away, ere break of day...",
"I hear the tramp of doom.",
"To fly would be folly.",
"The wind came down from mountains cold.",
"Cut the bridges! To arms! To arms!",
"Aah!",
"O! wandering folk, the summons heed!",
"I go now to the halls of waiting.",
"This is a bitter adventure, if it must end so.",
"I leave now all gold and silver, and go where it is of no worth.",
"But sad or merry, I must leave it now. Farewell!",
"His armour is shivered, his splendour is humbled.",
"Roads go ever ever on.",
"Yet feet that wandering have gone, turn at last to home afar.",
"Presumed dead.",
"For blood ye shall render blood.",
"The Shadow does not hold sway yet.",
"...and in the Darkness bind them.",
"One Ring to rule them all, One Ring to find them...",
"Leithio i philinn!",
"Fire the arrows!",
"Sedho, hodo.",
"Be still, lie still.",
"There-is-no life in-the-cold, in-the-dark.",
"Here - in-the-void only death.",
"I have kept no hope for myself.",
"I Aear can ven na mar.",
"The Sea calls us home.",
"Aiya Earendil Elenion Ancalima!",
"Hail Earendil brightest of the Stars!",
"Farewell sweet earth and northern sky.",
"Mourn not overmuch! Mighty was the fallen.",
"When winter comes without a spring that I shall ever see.",
"O! Rowan dead, upon your head your hair is dry and grey.",
"Out of doubt, out of dark, to the day's rising.",
"Hope he rekindled, and in hope ended.",
"Over death, over dread, over doom lifted.",
"Out of loss, out of life, unto long glory.",
"Now for wrath, now for ruin and a red nightfall!",
"Death in the morning and at day's ending...",
"For into darkness fell his star...",
"Where'd everybody go?",
"Caveyard! Paveyard!",
"Can't we talk this over?",
"Thy nuncle was dead as a lump o' lead.",
"A bump o' the boot on the seat.",
"I stood upon the bridge alone.",
"Dark is the path appointed for thee.",
"Thy heart shall then rest in the forest no more.",
"The Winter comes, the bare and leafless Day.",
"Where now the horse and the rider?",
"Who shall gather the smoke of the dead wood burning?",
"For our days are ending and our years failing.",
"I saw him walk in empty lands, until he passed away.",
"His cloven shield, his broken sword, they to the water brought.",
"It is death, my boy: they have deceived me.",
"His limbs they laid to rest.",
"I will not say the Day is done, nor bid the Stars farewell.",
"The shadow lies upon my tomb...",
"My cup runneth over...",
"I have drained my cup to the bitter dregs.",
"Si man i yulma nin enquantuva?",
"Namarie!",
"Fell deeds awake: fire and slaughter!",
"Cold be hand and heart and bone...",
"...cold be sleep under stone.",
"Never more to wake on stony bed...",
"Till the Sun fails and the Moon is dead.",
"In the black wind the stars shall die...",
"Out into the barren lands.",
"Where is my spellbook of Hand of Doom?",
"But I have double resist!",
"I hate the Random Number Generator.",
"Oh. I didn't know it could resist that.",
"I knew I should have played with stupid monsters on.",
"I knew I shouldn't have let monsters learn from their mistakes.",
"OK, turning that option on was a bad idea.",
"Ha, ha, wrong again.",
"This was supposed to be easy.",
"I don't get it.",
"But I read the manual!",
"*sigh*",
"My descendants will avenge me.",
"I should have listened to my ancestors.",
"I should have read the help files.",
"One ill turn deserves another.",
"That's not what the Wand of Death was supposed to do.",
"*whimper*",
"No, this really isn't happening.",
"*bang*",
"This can't be real.",
"Someone call the Shiriff!",
"Awake! Awake! Fear, Fire, Foes! Awake!",
"I didn't know it was a Scroll of Aggravate Monster!",
"New blood joins this earth...",
"But my Saving Throw is Good!",
"I really only like writing poetry.",
"What is Time, friend or foe?",
"Time waits for none.",
"Why didn't I start a Necromancer?",
"Flash before my eyes: now it's time to die.",
"Gaaaah! This is most frustrating.",
"I hate summoners.",
"I'm not dead yet.  I still have five hit points.",
"To sleep, perchance to dream.",
"This thing all things devours.",
"What's a burrahobbit?",
"Blackheart! I have done thy bidding.",
"Hail Sauron, Lord of the Ring, Lord of the Earth!",
"I'll be back for my revenge.",
"You have robbed my revenge of sweetness.",
"I'll see your descendants later!",
"Oh, great.",
"But, Sharkey!",
"But I'm famous!",
"I like happy things. I'm really calm and peaceful.",
"Won't you look at all the pretty flowers?",
"What treachery is this?",
"Down here, we all float.",
"See you in the Void!",
"No, no, no. It's not fair.",
"This didn't just happen.",
"You hit me for HOW MUCH damage?",
"You cheated!",
"Oh well, it's back to square one in HMa.",
"Great. Now I can finish my memoirs.",
"Sharkey made me do it.",
"This is just gruesome.",
"I believe my kingdom will come.",
"I'll see your descendants later!",
"Duh! Lev Zakrevski wouldn't have fallen for that one.",
"It's appalling.",
"Oh, basely done! I had hoped for better of thee!",
"I'll get even! Next time I see you, Death is yours!",
"Empire Micro$oft is eternal!",
"My God, Please visit a punishment upon this unrighteous sinner!",
"Faugh, I'll be back...so long as human psyche has a blackness. Ho-ho.",
"This CAN'T be happening! I am Dying....!!!!!",
"I desire peace ... for all people...",
"I finished my life with no regrets!",
"Ah, PosChengband is too difficult!  I'm going back to Chengband!!",
"I guess PosChengband is too easy!",
"I guess this is the last version of PosChengband!",
"Well, Hengband was much more fun than this!",
"Damn that AmyBSOD harlot, she's such a terribly bad Game Master!",
"It breathes--More--",
"Argh, fire enchanted explosion got me.",
"That bastard instakilled me!",
"I got instakilled again!",
"Gah, the bastard had a gun!",
"Here lies a snail that got salted to death by Semzov the Cleaner.",
"I made the mistake to gaze directly at Medusa.",
"There is no corpse in this grave because the dude died to a black dragon breath.",
"Go Team Ampersand!",
"The Three Big S got me - I died of sickness.",
"The Three Big S got me - I died of stoning.",
"The Three Big S got me - I died of sliming.",
"I made the mistake to use a tightrope on myself.",
"I died by stepping into an iron maiden.",
"Here lies a victim of the guillotine.",
"Died of neurofibroma.",
"Starved to death. Next time I'm playing a race that does not have intrinsic regeneration.",
"Here lies a poor explorer, all because he attacked an exploder.",
"C is for Clair who wasted away.",
"D is for Desmond thrown out of the sleigh.",
"E is for Ernest who choked on a peach.",
"F is for Fanny, sucked dry by a leech.",
"G is for George, smothered under a rug.",
"H is for Hector, done in by a thug.",
"I is for Ida who drowned in the lake.",
"J is for James who took lye, by mistake.",
"K is for Kate who was struck with an axe.",
"L is for Leo who swallowed some tacks.",
"M is for Maud who was swept out to sea.",
"N is for Nevil who died of ennui.",
"O is for Olive, run through with an awl.",
"P is for Prue, trampled flat in a brawl.",
"Q is for Quinton who sank in a mire.",
"R is for Rhoda, consumed by a fire.",
"S is for Susan who perished of fits.",
"T is for Titas who blew into bits.",
"U is for Una who slipped down a drain.",
"V is for Victor, squashed under a train.",
"W is for Winie, embedded in ice.",
"X is for Xerxes, devoured by mice.",
"Y is for Yoric whose head was bashed in.",
"Z is for Zilla who drank too much gin.",
"I played FIQhack and got to the quest but then some asshole read a scroll of genocide and wiped me out.",
"I played Nethack Fourk but a deep ettin summoned an island nymph who stole my shield of reflection and jumped into a hole, and then a lieutenant with a wand of death appeared.",
"Tried to punch out Cthulhu in Dynahack. He sucked my brain 200 times and then I died.",
"The elder priest in dnethack killed me, that cheater!",
"Tried to eat a tainted giant corpse in Grunthack. And then the game crashed while trying to make a bones file. Joy.",
"The wand destruction patch (which is bullshit) killed my Nethack Fourk character! Whose bright idea was it to make lightning bypass my reflection and vaporize my ring of levitation while I'm over open water?",
"The wand destruction patch (which is bullshit) killed my Nethack Fourk character! Whose bright idea was it to make wands of teleportation not work at all on the castle where you have no hopes of fighting an ancient dragon head-on?",
"This unfortunate fellow engaged the elder priest in dnethack and had all his stuff corroded, then lost his cloak of magic resistance to the asshole tentacles, and while trying to slip on the ring of levitation to get past the 200 pits the priest created, made the mistake of taking off free action and was chain-paralyzed to death.",
"A person who inhaled too much of Katharina's fart gas.",
"Tried to play Dynahack and got a repeatable crash upon trying to kill Rodney, so I decided to commit sudoku.",
"Unnethack's Aphrodite (who certainly isn't overpowered at all) and her bitches stole all of my belongings and then a tiger spawned to assault my naked self. Yes, I could have engraved Elbereth, but it wasn't really worth it anymore.",
"I made the mistake of playing SLASHTHEM. Got bored to tears after five minutes, and resorted to repeatedly throw my sword upward after ten minutes.",
"Someone told me that there was a cure for acute depression waiting for me at the train station in 10 minutes, so I went ahead and started walking on the railroad tracks.",
"I played SLASHTHEM and died to some bullshit. Be glad you're playing a much better variant instead!",
"I played SLASHTHEM with my favorite role from SLASH'EM Extended and realized that someone had nuked the most defining features of that role. Not wanting to put up with that bullshit, I shot my rifle in . direction repeatedly.",
"I played FIQhack. The last thing I saw was a kobold shaman and a few dozen instances of 'The sleep ray hits you!' on my message log culminating in 'You die--More--'.",
"I played Sporkhack and started in the lower right corner surrounded by rock. My pick-axe only produced the message 'This wall is too hard to dig into', so I applied it to my own skull instead.",
"The orange dragon technically instakilled me from full health because I stupidly thought that 54%% sleep resistance is enough. God I hate Sporkhack!",
"This dude fought zombies in Grunthack and eventually became one himself.",
"What, why did that succubus kill me? There was no message about her attacking or anything! What the hell, Nethack Fourk? I just died out of the blue!",
"No corpse can be found in this grave, because this grave is of a player who tried to take on an ancient black dragon in Nethack Fourk thinking that disintegration resistance would protect the inventory. He learned the hard way that it does not.",
"A scurrier tourist would have been buried here, but Nethack Fourk crashed and warped the body to the segfaultitis realm.",
"This samurai tried to fight an enemy samurai that had a wand of lightning, not knowing that Dynahack changed shock resistance to only offer 50%% protection.",
"Killed by bumping into an invisible shopkeeper.",
"Killed by foolishly trying to melee a superjedi at experience level 2.",
"Killed by foolishly trying to melee a giant eel at experience and dungeon level 2.",
"Wait, why did the priest get hostile? I got a strange feeling of peace when entering the te-- oh shit, I forgot that altars can disappear when dropping stuff on them...",
"Huh. Didn't think the darts of disintegration thrown by that kobold would actually disintegrate me. Oh well, at least my tombstone reads 'killed by a died', which sounds really cool.",
"Killed by a sexy leather pump.",
"Killed by bleedout from leg-scratches.",
"Instead of a corpse, we buried a statue here - it was created because some intelligent monster threw a cockatrice egg at the alive body.",
"Tried to eat a dreamer cockatrice, and forgot that the paralysis only happens after eating it is complete.",
"A poor soul who tried to eat an eight-footed snake, not knowing that they cause petrification.",
"Wait, so if I eat an undead mimic and become deathly sick, I end up becoming a gold piece for more turns than it takes for the sickness to kill me???",
"AmyBSOD: Enduring dnethack, until the inevitable death to the MOTHER FUCKER THAT WAS INSERTED BY A DEVELOPER WHO HATES EVERY LIVING BEING.",
"The elder priest in dnethack: No one has ever killed him, no one will ever kill him and those who try are dead. Like me.",
"'the elder priest of Moloch is a contender for the title of second most dangerous monster in dnethack' my ass, he's the SINGLE MOST DANGEROUS MONSTER THAT HAS EVER GRACED A PC SCREEN",
"Killed in dnethack by a beautiful pair of high-heeled stilettos. If you see Chris, please tell him that I want to hug him for adding the wonderful high heels!",
"Killed by dnethack's elder priest. I will now go ahead and write page after page of revenge porn.",
"fuck that monster!",
"ugods unbalanced fix plz",
"truck ghosts are unfair!!!!!",
"waaaaaaahhhhhhh something zapped a wand of remove resistance and stole my doppelgangers polymorph control i cannot play on without it waaaaaaaaaahhhhhhhh",
"What? The game didn't give me a sickness message when my angel kurwa repeatedly meleed that gray growth!",
"'Fatal contamination'? Doesn't seem to do all that mu-",
"Huh. Didn't expect that 'fatal contamination' causes me to grow a third leg.",
"I underestimated SLEX's much more dangerous Team K and got killed by a thrown cream pie.",
"Wait, so I'm not supposed to walk around in weird-looking rooms?",
"Why is there a special room type that's more unfair than any others and why is it the U-filled terror hall?",
"Drew the maze level with dozens of special rooms back to back on dlvl2 and died to a ginormous squad of trolls. But hey, there were 600+ monsters generated in a game that only lasted 500 turns!",
"Huh. I thought the temple of Moloch was the place you go to if you want to buy divine protection.",
"Why did the priest on the Lawful Quest turn on me? He was coaligned and I had 50k zorkmids!!!",
"Why did that imp Flickerer fail to die after I hit him with my long sword 50 times????????",
"'Stairs trap' my ass.",
"Vaporization attacks are gay.",
"Well screw this retarded game too.",
"What the hell was the developer woman smoking???",
"Whoa, didn't expect the thick farting girl to own me that hard. She surely didn't look that dangerous when I looked at her stats...",
"I died to a GruntHack beholder because I didn't realize that the warning levels ONLY display the average level a monster has and not whether it's dangerous, so I stupidly thought a 2 on the warning meter couldn't pose a threat.",
"The air elemental speed beholder got me. Watch out, it's probably still around somewhere close by.",
"Air elementals should be renamed to 'fuck you'.",
"Monsters moving and shooting the same turn is so unfair!",
"The game broke down AGAIN!",
"I think centaur princes should not be allowed to fire 5 crossbow bolts per turn.",
"Committed suicide after hitting a superscroller trap. Amy, remove that trap from the game or I won't play it again.",
"Committed suicide after failing 50 times in a row to tame a wounding knight. Why didn't I simply say 'screw it' and move on? I have no idea.",
"Why were my sling rocks not doing any dama- DUH, forgot to actually wield that sling!",
"What, void room on dungeon level 5? With truck ghosts??? Seriously, Amy?",
"Amy I will never play this game again!",
"this game is unfair and kills you in random ways",
"shopkeepers should not be spawned invisible fix plzzzzz",
"amy you must remove nasty traps from the game",
"amy this thing killed me please remove it",
"amy i want this game to be as easy as vanilla nethack please remove everything that can kill the player character",
"Why is being drowned by a giant eel an instadeath? I had FULL health left!",
"What, digging in > direction while standing next to lava is an instadeath? That's bullshit.",
"You made explosive boots deal 200+ damage when they explode??? That's insane, no XL10 character will even have that many to begin with!",
"Wow, Jane's dancing shoes really are elegant... I couldn't help it, all I could do was wanking off while they kicked me to death.",
"I inhaled the beguiling stench of Josefine's sweaty feet until I fell down unconscious, and then she killed me with her velcro sneakers before I came to myself again.",
"Got my balls kicked by an asian woman in stiletto heels. She kicked them so hard, they were force-pushed out of their usual position causing several internal ruptures and internal bleeding which eventually proved fatal.",
"Killed by repeatedly being whacked over the head by Natalia's wedge sandals for being a very, VERY naughty boy!",
"Breath control fetishism is what killed this person.",
"Died of erotic asphyxiation.",
"Died of erotic asphyxiation. Turned out that yes, you can suffocate from tying worn socks to your mouth and nose if you use enough of them at once.",
"Had my x and y reversed.",
"Electrocuted by a queevolt.",
"Molested like a loli by the elder priest's tentacles.",
"stupid water tiles!",
"monsters should not spawn with wands of disintegration before level 20 because i noob and i forgot that items not have levels in this game",

	"TANJ!", /* Larry Niven */
	"Summer Rose - Thus Kindly I Scatter", /* RWBY */
	"Hold my beer and watch this!", /* Rango */
	"Of course, you realize, this means war.",
	"In death ground, fight.", /* Sun Tzu */
	"Did you see him passing by?", /* Dark Souls 3 */
		"The world began without knowledge, and without knowledge will it end.", /* Dark Souls */
		"Fear not the dark, my friend. And let the feast begin.",
	"For every hero commemorated, a thousand martyrs die unmourned and unremembered." /*Warhammer 40k*/
	"FIQ:  One step from divinity.",
		"FIQ:  Debugging dNethack, one iron ball to the face at a time.",
		"FIQ, killed by a clockwork bug.",
	"Khor:  \"Wait, did that axe just fall off the edge of the world?\"",
		"Khor:  Balancing dNethack, one sleeping potion to the face at a time.",
	"AmyBSoD: \"Phew, killed that elder priest.  He shredded my CoMR but at least he's de-\"",
	"Package contains: One posthumous worm feeder",
	"Was that the hill I wanted to die on?",
	"Used no net, knew no fear, made mis-step, wound up here.", /*Balder's Gate gravestones*/
		"Here lies the body of Rob.  If not, please notify the undertakers at once.",
		"When I can no longer stand alone, then it will be time to die.",
		"I feel my body rising towards the bright light...wait, now it's falling, What the HELL!",
		"Reader if cash thou art in want of any, dig four feet deep and find a penny",
		"A lesson learned. Too bad I'm dead.",
	"I never got a single answer to any of my biggest questions, and now I will never know.", /*Unicorn Jelly*/
	"I beat you in the human race.", /*Fable*/
	"Nobody expects the Spanish Inquisition!", /*Monty Python*/
	"Omnes una manet nox", /*One night awaits us all*/
	"I wake. I work. I sleep. I die." /*Alpha Centauri*/
	"I suddenly have a LOT of regrets.", /*Water Phoenix King is very quoteable...*/
	"You either die a hero, or you live long enough to see yourself become the villain.", /* Batman, the Dark Knight */
	"What can men do against such reckless hate?", /* the Lord of the Rings (Two Towers movie) */
	"I'll just hit this thing a couple more times, it'll die.", /*jonadab*/
		"I don't need to unburden myself, they're only soldiers, I've been killing soldiers, it'll be fine.",
	"Maybe if I press this key a bit harder I'll hit for more damage next time.", /*Grasshopper*/
	"...nor the battle to the strong", /*Bible*/
	"fucking yora chick suck dick.", /* by GoldenIvy */
	"looks like it's get the fuck out of here o'clock", /* by Khor */
	"It's a kick in the pants when you get killed by your spouse for playing the piano.",
	"that is why the dnethack elder priest exists - to simply frustrate everyone", /* by rikersan, and I couldn't agree more (as if that wasn't obvious from all the other messages about that mother fucker tentacle asshole :P) */
	"Forgot to wear a cloak, and then the wight used his level-draining attack a couple of times.",
	"What? I didn't know that thing autocurses!",
	"WTF, the hippie heels autocurse even if they are blessed? And they convey extrinsic hallucination? That's unfair!!!",
	"Here lies the n00b that decided to ragequit after his doppelganger was zapped with remove resistance and lost intrinsic polymorph control.",
	"Here lies a n00b that used #quit on dungeon level 25 after suffering what should have been only a minor setback.",
	"Please don't spawn whitlevel kops next time!",
	"Well, the void room is more than just a little unbalanced.",
	"Committed shoplifting and levelported away. How did the kops get from dlvl5 to 35 SO fast???", /* many thanks to Elronnd for the inspiration */
	"Why is the undead katcher not undead? I would have lived if he was, I had command undead at 0%% fail!",
	"Unfair attack trap. And then a normally harmless gnome lord randomly decided to get a touch of death off.",
	"I was too impatient. I had gotten the Amulet of Yendor already, but on the way up I ESCed out of the messages telling me about the succubus taking off my silver dragon scale mail, and then a black dragon breathed disintegration.",
	"Well, apparently eating up all the K-rations before hitting the Astral Plane is not the best idea if you then run out of permafood while fighting Famine.",
	"Whoa, I completely forgot that Pestilence can do up to 128 damage per round. I thought he couldn't touch me, I had a blessed unicorn horn after all.",
	"Wait, so you're saying there is a defense against random monsters with a wand of disintegration that appear out of nowhere? I don't believe that, all I saw was 'you hear a nearby zap' followed by the DYWYPI prompt!",
	"Scrolls of flood are bullshit on Sokoban. #quit",
	"You can't play with the yellow spell trap. I decided to #(rage)quit upon triggering it, because I'm convinced that makes me a better player.",
	"You can't play with the yellow spell trap. I decided to #(rage)quit upon triggering it, because I'm not touching SLASH'EM Extended again. I'll play casual games like Pac-Man or Minesweeper instead, since I need to polish my ego and that whole concept of 'difficult games' is too much for me to handle.",
	"The counterclockwise spin trap screwed me over, without it I'd have hit the correct button and walked away from the monster instead of bumping into it!",

};

const char *
random_epitaph()
{
	return (epitaphs[rn2(SIZE(epitaphs))]);
}


/* Create a headstone at the given location.
 * The caller is responsible for newsym(x, y).
 */
void
make_grave(x, y, str)
int x, y;
const char *str;
{
	/* Can we put a grave here? */
	if ((levl[x][y].typ != ROOM && levl[x][y].typ != GRAVE && levl[x][y].typ != CORR) || t_at(x,y)) return;

	/* Make the grave */
	levl[x][y].typ = GRAVE;

	/* Engrave the headstone */
	if (!str) str = epitaphs[rn2(SIZE(epitaphs))];

	del_engr_at(x, y);
	make_engr_at(x, y, str, 0L, HEADSTONE);
	return;
}


#endif /* OVLB */

/*engrave.c*/
