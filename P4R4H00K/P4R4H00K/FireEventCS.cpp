#include "Includes.h"
#include <random>
#include <vector>


FireEvent g_fnOriginalFireEvent = nullptr;

std::vector<std::string> jokesSentences = {
	"Why can't you play Uno with a Mexican? They steal all the green cards.",
	"Father's day, the most confusing day in the ghetto.",
	"Why doesn't Mexico have an Olympic team? Because everybody who can run, jump and swim are already in the U.S.",
	"What would martin luther king be if he was white? Alive.",
	"Why dont blacks celebrate thanksgiving? KFC isnt open on holidays.",
	"Why is there cotton in pill bottles? To remind black people that they were cotton pickers before drug dealers.",
	"How can you tell when the Mexicans have moved into your neighborhood? The Blacks get car insurance.",
	"Why wasnt there any blacks in the flintstones? Because they were still monkeys.",
	"What did the little Mexican boy get for christmas? My bike.",
	"What does it mean when you see a bunch of blacks running in one direction? Jail break.",
	"What do you call a bunch of dead black people in a barn? Out dated farming equipment.",
	"What do you call a black priest? Holy Shit.",
	"How long does it take a black lady to shit? About 9 months.",
	"Why do blacks wear white gloves? So they don't bite their fingers eating tootsie rolls.",
	"What's long, black and smelly? The unemployment line.",
	"Hey, I'm not saying Hitler was a great guy, but he really saved the Histoy channel.",
	"Why do blacks smell? So blind people can hate them too.",
	"Why are all blacks fast? The slow ones are in jail.",
	"What is the difference between batman and a black man? Batman can go into a store without robin.",
	"What do you call a bench full of white people? The MLB.",
	"What is the difference between a jew and a pizza? A pizza doesn't scream when you put it in the oven.",
	"What did God say when he saw the first black person? Ooops, I burnt on.",
	"Why don't blacks fuck Afghans? Because they are going to make you blow.",
	"What is yellow and black and makes you laugh ? A bus full of niggers going over a cliff.",
	"Whats a Mexicans favourite sport? Cross Country.",
	"What do u call a black guy with a peg leg? shit on a stick",
	"What do you call a white man in the ghetto? A victim.",
	"What is the difference from a nigger an a bucket of shit? The bucket.",
	"What do you call it when a white man marries the woman of his dreams? Incest.",
	"What do you call it when a white man dancing has a seizure? An improvement.",
	"What is the difference between Michael Jackson and a grocery bag? One is white, made out of plastic, and dangerous for kids to play with and the other you carry your groceries in.",
	"How is a nigger like a broken gun? It doesn't work and you can't fire it.",
	"What is the difference between a pothole and a nigger? You'd swerve to avoid a pothole, wouldn't you?",
	"I bought a black computer because I thought it would run faster.",
	"You know you're screwed when your Asian classmate says 'shit' during the test.",
	"Black man: Siri, take me home. Siri: Starting route to the nearest jail.",
	"How do you watch black spongebob? On niggalodeon.",
	"Whats the difference between a bullet and a Jew... A bullet leaves the chamber",
	"If Steve has $20 and Tyrone takes $16, what color is Tyrone?",
	"Teacher: Tyrone, spell dad. Tyrone: G-O-N-E.",
	"I once entered an Arabic clothing shop but they kicked me out for asking if they had a bomber jacket.",
	"What do black people and sperm have in common? Only one in a million work.",
	"How many police officers does it take to change a light bulb? None, they beat the room for being black.",
	"Once you go black you can always come back, with the Mr. Clean Magic Eraser.",
	"Why do black people hate chainsaws? because they go runigganigaanigganigga.",
	"I was going to make a black person joke but someone stole it",
	"Why did God give black people big dicks? He felt sorry for the pubic hair on their head",
	"I would make a Mexican joke, but it would cross the border.",
	"Why does Beyonce sing 'To the left, to the left' ? Because black people have no rights" };
std::vector<std::string> flameSentences = {
	"You pay for that ?",
	"Nice box faggot",
	"Nice edge nigger",
	"Go back to africa dumb refugee",
	"Can smell you from over here",
	"l2p indian tech fag",
	"1",
	"I think you lost some chromosomes",
	"Nice baim faggot",
	"Getting terminal cancer from your low IQ",
	"Hitler is my father and he send me to kill all the jews",
	"U even cheating bro?",
	"mad?",
	"Nice pasta",
	"Great esp",
	"Nice resolver u sell",
	"You use ezfrags?",
	"What hitscan?",
	"P4R4H00K, refugee slayer",
	"I shit a nigger out by just looking at your aa",
	"Cya nerd send me a card from rekttown rofl",
	"To which foundation do I need to donate to help you?",
	"You should commit suicide. That way you'd have at least one kill.",
	"I can't wait till I have dementia so I can forget that ever happened.",
	"Even Noah can't carry these animals",
	"With an aim like that, I'm surprised you were able to turn your PC on.",
	"Play with a blindfold on, at least you'll have a valid reason for missing all those shots.",
	"I bet your club penguin pets killed themselves",
	"You have worse map awareness than Christopher fucking Columbus.",
	"Too bad you weren't in control of the Titanic. You would have missed the Iceberg.",
	"You must really like that respawn timer",
	"Some babies were dropped on their heads but you were clearly thrown at a wall",
	"I could eat alphabet soup and still shit out a better argument than you.",
	"Your kills are like all Valve franchises, stuck at 2",
	"I'm not trash talking, I'm talking to trash",
	"What's the difference between you and a bench? The bench can actually support an entire team.",
	"Shut up, you'll never be the man your mother is.",
	"If you want to hear about mistakes, ask your parents.",
	"I recommend you keep crying. It's the only thing you're doing right.",
	"The keys you're looking for are Alt-F4",
	"only thing u carry is an extra chromosome",
	"You must have been born on a highway, because that's where most accidents happen..",
	"If only you could hit an enemy as much as your dad hits you.",
	"'404' Skill not found",
	"Don't trip on your way into respawn",
	"If I wanted to kill myself I would jump from your ego down to your IQ",
	"I'm glad to see you took a break from Cookie Clicker to grace us with your MANY talents",
	"You're the reason your parents fight",
	"Hey, give me a shoutout to your 2 Twitch viewers",
	"No this isn't Pay-To-Win, all you have to do is pay attention",
	"Keep rolling your eyes, maybe you'll find a brain back there",
	"I guess the abortion clinic forgot one",
	"If you keep crying, we're going to need to build another Ark.",
	"Guys, can we put the bots on a higher difficulty?",
	"aww, you're just like Jesus, always respawning and never hurting anyone :)",
	"Go back to somalia with your pBoat" };


//Hook for the events of the game
bool __fastcall Hookers::Hooked_FireEvent(void* ecx, void* edx, SSDK::IGameEvent* pEvent)
{
	if (!pEvent)
		return g_fnOriginalFireEvent(ecx, pEvent);


	//*****SI ON TUE
	if (!strcmp(pEvent->GetName(), "player_death"))
	{
		// The game launched a player_death event.
		int nUserID = pEvent->GetInt("attacker");
		int nDead = pEvent->GetInt("userid");
		if (!nUserID || !nDead)
			return false;

		C_CSPlayer* hurt = (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(SSDK::I::Engine()->GetPlayerForUserID(nDead));
		C_CSPlayer* attacker = (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(SSDK::I::Engine()->GetPlayerForUserID(nUserID));
		C_CSPlayer* pLocalPlayer = C_CSPlayer::GetLocalPlayer();

		//MESSAGE SI ON SE FAIT TUER
		if (hurt == pLocalPlayer || attacker == pLocalPlayer)
		{
			bool gotKilled = (hurt == pLocalPlayer);


			//CHATBOT
			if (MenuOptions.Misc.g_chatbot)
			{
				static std::random_device randomSentences;
						
				if (MenuOptions.Misc.g_chatbotKillMode == 1 && !gotKilled)
				{
					//SSDK::PlayerInfo killed_info = Utils::GetInfo(SSDK::I::Engine()->GetPlayerForUserID(nDead));

					//std::stringstream ss;

					//ss << "say " << killed_info.szName << ", you got rekt by P4R4H00K.XYZ";

					SSDK::I::Engine()->ExecuteClientCmd("You got rekt by P4R4H00K.XYZ");
				}

				if ((MenuOptions.Misc.g_chatbotDeadMode == 1 && gotKilled) || (MenuOptions.Misc.g_chatbotKillMode == 2 && !gotKilled))
				{
					std::default_random_engine rndEngine(randomSentences());
					std::uniform_int_distribution<int> uniform_dist(0, jokesSentences.size() - 1);
					int sentenceNumber = uniform_dist(rndEngine);

					SSDK::I::Engine()->ClientCmd_Unrestricted(("say " + jokesSentences[sentenceNumber]).c_str());
				}
				else if ((MenuOptions.Misc.g_chatbotDeadMode == 2 && gotKilled) || (MenuOptions.Misc.g_chatbotKillMode == 3 && !gotKilled))
				{
					std::default_random_engine rndEngine(randomSentences());
					std::uniform_int_distribution<int> uniform_dist(0, flameSentences.size() - 1);
					int sentenceNumber = uniform_dist(rndEngine);

					SSDK::I::Engine()->ClientCmd_Unrestricted(("say " + flameSentences[sentenceNumber]).c_str());
				}
				//SSDK::I::Engine()->ExecuteClientCmd("say Wait...you actually killed me? How dare...");
				//return false;
			}
		}
		//SI ON PLANTE
		if (!strcmp(pEvent->GetName(), "bomb_planted"))
		{
			if (MenuOptions.Misc.g_generalChatbot)
			{

				// The game launched the bomb_beginplant event
				int nPlanter = pEvent->GetInt("userid");
				if (!nPlanter)
					return false;

				SSDK::PlayerInfo planter_info = Utils::GetInfo(SSDK::I::Engine()->GetPlayerForUserID(nPlanter));

				std::stringstream ss;
				ss << "say Wow, " << planter_info.szName << ", you planted the bomb like a real muslim!";

				SSDK::I::Engine()->ExecuteClientCmd(ss.str().c_str());

			}
		}

		//SI LA BOMBE EXPLOSE
		if (!strcmp(pEvent->GetName(), "bomb_exploded"))
		{
			if (MenuOptions.Misc.g_generalChatbot)
			{
				// The game launched the bomb_exploded event
				SSDK::I::Engine()->ExecuteClientCmd("say ALLAHU AKBAR!!!");
			}
		}

		//if (!strcmp(pEvent->GetName(), "bomb_dropped"))
		//{
		//	int nDropper = pEvent->GetInt("userid");
		//	if (!nDropper)
		//		return false;

		//	player_info_t info = GetInfo(SSDK::I::Engine()->GetPlayerForUserID(nDropper));

		//	std::stringstream ss;
		//	ss << "say Oh no! " << info.szName << " dropped the fuckin bomb!";

		//	SSDK::I::Engine()->ExecuteClientCmd(ss.str().c_str());
		//}

		//if (!strcmp(pEvent->GetName(), "bomb_defused"))
		//{
		//	int nDefuser = pEvent->GetInt("userid");
		//	if (!nDefuser)
		//		return false;

		//	player_info_t info = GetInfo(SSDK::I::Engine()->GetPlayerForUserID(nDefuser));

		//	std::stringstream ss;

		//	ss << "say " << info.szName << " defused the bomb with SWAT style! Bin Laden really sad atm...";

		//	SSDK::I::Engine()->ExecuteClientCmd(ss.str().c_str());
		//}

		//SI ON DEFUSE
		if (!strcmp(pEvent->GetName(), "bomb_begindefuse"))
		{
			if (MenuOptions.Misc.g_generalChatbot)
			{
				//[WARNING: This event isn't called anymore, idk why]
				int nDefuser = pEvent->GetInt("userid");
				if (!nDefuser)
					return false;

				SSDK::PlayerInfo info = Utils::GetInfo(SSDK::I::Engine()->GetPlayerForUserID(nDefuser));

				std::stringstream ss;

				ss << "say " << info.szName << " started defusing the mothafucking bomb!";
				if (pEvent->GetInt("haskit"))
					ss << " And he has got a l33t defuser!!!";

				SSDK::I::Engine()->ExecuteClientCmd(ss.str().c_str());
			}
		}

		//SI ON ARETTE DE DEFUSE
		if (!strcmp(pEvent->GetName(), "bomb_abortdefuse"))
		{
			if (MenuOptions.Misc.g_generalChatbot)
			{
				//[WARNING: This event isn't called anymore, idk why]
				int nAborter = pEvent->GetInt("userid");
				if (!nAborter)
					return false;

				SSDK::PlayerInfo info = Utils::GetInfo(SSDK::I::Engine()->GetPlayerForUserID(nAborter));

				// Continue only if I'm not the aborter
				if (SSDK::I::Engine()->GetPlayerForUserID(nAborter) == SSDK::I::Engine()->GetLocalPlayer())
					return false;

				std::stringstream ss;

				ss << "say " << info.szName << " aborted the bomb defusal like a fucking pussy!!!";

				SSDK::I::Engine()->ExecuteClientCmd(ss.str().c_str());
			}
		}

		//if (!strcmp(pEvent->GetName(), "round_freeze_end"))
		//{
		//	SSDK::I::Engine()->ExecuteClientCmd("say Wow another round full of pure skills starts!");
		//}
	}

	return g_fnOriginalFireEvent(ecx, pEvent);
}