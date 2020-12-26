#include <ctime>
#include <cstdlib>


//#include "tenthousand.hpp"
#include "sarsa.hpp"

#include <windows.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <conio.h>

#ifdef TEST_RUN
    #error TEST_RUN is already defined
#else
    #define TEST_RUN
#endif // TEST_RUN

    // game
#ifdef TEST_RUN
    int constexpr playerNo = 2;
#else
    int constexpr playerNo = 3;
#endif

double avg[playerNo] = {0};
int tries[playerNo] = {0};


double updateAvg(int pl, double total)
{
    ++tries[pl];
    return avg[pl] = total / tries[pl];
}

std::string dToS(double d)
{
    std::ostringstream oss;
    oss << d;
    return oss.str();
}

/// \tbc make an unbiased randomness generation for the dice rolls


/// \tbc define #TEST and #NOTEST to execute depending on global constexpr?

using namespace std;

template<unsigned int n>
static void reprint(Tenthousand<n> const& game, string msg = "")
{
    system("cls");
    cout << std::setprecision(5) << std::fixed << msg << endl;
    cout << string(80,'-') << endl;
    game.print();
}

void gradDesc_hyperparameter();
double evalParams(double,double,double,double,int,int);

int main()
{
    std::srand( std::time(nullptr) );
//    gradDesc_hyperparameter();
    /// \tbc test / read copy constructor calls
    /// \tbc implement random state learning

    /// \tbc make strengths < 100 TRAIN with 350 rule but play without? :D they don't know better ^^
    /// play 1 human vs. 2 bots

#ifndef TEST_RUN
    cout << "In-Game controls:\n"
         << string(72, '-') << endl;
    cout << setw(14) << std::left << "(1) to (6)" << "select die (deactivates it)\n"
         << setw(14) << std::left << "(a)" << "select All possible dice\n"
         << setw(14) << std::left << "(r)" << "Roll active dice\n"
         << setw(14) << std::left << "(f)" << "Finish your turn\n"
         << setw(14) << std::left << "(l)" << "get Lost (quit)\n" << endl;
    cout << "\tNote: (a) Can find new triples, but is currently unable\n\tto complete triples\n" << endl;
    cout << setw(14) << std::left << "\nPress any key to start." << endl;
    getch(); /// \tbc quick = use setting from last timeS

    // computer interaction (input parameters by player)
    system("cls");
    cout << "Choose your AI level.\n"
         << string(72, '-') << endl
         << "Note:\n\tCurrently, AI skill is limited by computing power.\n"
            "\tYou might still win against your local desktop machine.\n\n"
            "Typical levels might be:\n\n\n" << std::right
         << "\t" << setw( 7 ) << 10     << "\t| newborn  | hopefully doesn't shit its pants"     << endl
         << "\t" << setw( 7 ) << " "    << "\t|          |" << endl
         << "\t" << setw( 7 ) << 100    << "\t| child    | might understand the rules next year" << endl
         << "\t" << setw( 7 ) << " "    << "\t|          |" << endl
         << "\t" << setw( 7 ) << 1000   << "\t| teenager | mostly distracted but sometimes lucky af" << endl
         << "\t" << setw( 7 ) << " "    << "\t|          |" << endl         << "\t" << setw( 7 ) << 10000  << "\t| adult    | an actual opponent"<< endl
         << "\t" << setw( 7 ) << " "    << "\t|          |" << endl
         << "\t" << setw( 7 ) << 100000 << "\t| R2FU     | that fkn bot has hax" << endl
         << "\n\n\tAny number from 0 to 4 billion will work.\n\n\n"
            "Level: ";
#else
    cout << "Starting test" << endl;
#endif
    int steps;
#ifndef TEST_RUN
    cin >> steps; cin.sync(); cin.clear();
    if( steps < 0 )
        steps = 0;
#else
    steps = 0;
#endif

    cout << "\n\n\tStarting. Please wait..." << endl;
    Sarsa* bot;
    Sarsa::Game_t* trainEnv = new Sarsa::Game_t();
#ifndef TEST_RUN
    if( steps < 1000 )
        bot = new Sarsa(0.25, .2);
    else if( steps < 10000 )
        bot = new Sarsa(0.18, .15);
    else if( steps < 100000 )
        bot = new Sarsa(0.14, .12);
    else
        bot = new Sarsa(0.12, .1);
#else
    bot = new Sarsa(trainEnv, 0.01, .15, 1., .6); // gamma = 0.99 implicit
#endif

#ifndef TEST_RUN
    system("cls");
    cout << "For level " << steps << ", the AI chose the following parameters for 1-step TD(0) control (Sarsa):\n"
            << "\n\talpha      = " << bot->alpha
            << "\n\tepsilon    = " << bot->epsilon
            << "\n\tgamma      = " << bot->gamma << endl;
    cout << "\n\n" << endl;
    // randomness
    std::srand(time(nullptr));
#else
    std::srand(time(0));

    cout << "NO TRAIN NO GAIN" << endl;
    for(int i =  50000000; i != 0; --i)
    {
        if( !(i % 1000000) ) cout << ".";
        bot->rl::SarsaMaxLambda::performEpisode();
    }
    cout << "DONE" << endl;


#endif

    // game
    Tenthousand<playerNo> game;

    // human interaction
    unsigned char in;

    // errors and info
    string s;

    // time management: thinking and waiting
    enum Activity { LEARN, PROCRAST };
    #ifndef TEST_RUN

    auto wait = [&bot, &steps](bool activity = PROCRAST, double factor = 1.0)->void
    {
        size_t start = time(nullptr);
        if( activity == LEARN )
        {
            int l = int(steps*factor),
                i = l;
            if( i < 0 )
            {
                i = 0; l= 0;
            }

            cout << "[Training]\t:\t" << l << endl;
            bot->performLearningEpisodes(l, 80);
            cout << endl;
        }

        int left = 4 - (time(nullptr) - start);
        left = left > 0 ? left : 0;
        #ifndef TEST_RUN
        Sleep(left * int(factor * 1000)); /// \tbc lol does not work as intended

        #endif // TEST_RUN
    };
    #endif // TEST_RUN

#ifndef TEST_RUN
cout << "[Preparation]" << endl;
    bot->performLearningEpisodes(steps * 10, 80);
#else
//    bot->performLearningEpisodes(500000000, 80);
//      bot->performLearningEpisodes(50000000, 80);


    // 500000000 (.05/.05) -> avg 430 (200000 total)
    // 500000000 (.06/.06) -> avg 454 (300000 total)
    // 500000000 (.07/.07) -> avg 446 (300000 total)
    // 500000000 (.09/.07) -> avg 360 (100000 total)
    // 50000000  (.05/.1)  -> avg 452 (200000 total) | SARSAMAX
    // 50000000  (.09/.09) -> avg 440 (200000 total) | SARSAMAX
    // 50000000  (.04/.11) -> avg 440 (200000 total) | SARSAMAX
    // 50000000  (.11/.11) -> avg 442 (200000 total) | SARSAMAX
    // 50000000  (0.01 .15 1. .6) -> avg 462 (200k total) | SARSAMAXLAMBDA


#endif

    // game loop
    cout << "\nStarting now" << endl;
    while(true)
    {

        try
        {
            reprint(game,s);

            if( game.getPlayer() != 0 )
            {
                // - humans turn
                if( game.getPlayer() == 1 )
                    if( game.getCurrent() == 0 )
                        updateAvg(0, game.getPoints(0));    // player 1 updates avg of bot on first turn
                s = std::string("(bot) ") + dToS(avg[0]) + " -- " + dToS(avg[1]) + " (hum) ";

                bool in_again = true;
                do
                {
#ifdef TEST_RUN
                    in = 'f';
#else
                    in = getch();
#endif // TEST_RUN

                    switch( in )
                    {
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                        game.putAside(in - '1');
                        in_again = false;
                        break;
                    case 'a':
                        game.putAside();
                        in_again = false;
                        break;
                    case 'f':
                        game.putAside();
                        game.finishTurn();
                        in_again = false;
                        break;
                    case 'r':
                        if( game.getPutAny() )
                        {
                            game.roll();
                            in_again = false;
                        }
                        break;
                    case 'l':
                        goto cleanup;
                    default:
                        cout << "[inv]" << endl;
                        continue;
                    }
                } while( in_again == true );
            }
            else
            {
                // - AI turn

                if( game.getCurrent() == 0 )
                    for(int i = 1; i < playerNo; ++i)
                        updateAvg(i, game.getPoints(i));    // update avg of other player

                s = std::string("(bot) ") + dToS(avg[0]) + " -- " + dToS(avg[1]) + " (hum) ";

                std::string m;

                Sarsa::State_t state( game.state() );

                #ifdef TEST_RUN
                Sarsa::Action_t a = bot->greedy( std::move(state), true );
                #else
                Sarsa::Action_t a = bot->greedy( std::move(state), false );
                #endif // TEST_RUN

                m += "\n\tThinking...\n";
                cout << m << endl;

                #ifndef TEST_RUN
                wait(LEARN);
                #else
//                bot->performLearningEpisodes(steps, 0);
                #endif // TEST_RUN

                if( Sarsa::Game_t::legal(state, a) != 0 )
                {
                    game.putAside( a );

                    m += "\n\tI do this.\n";
                    reprint(game);
                    cout << m << endl;
                    #ifndef TEST_RUN
                    wait(PROCRAST, .25);
                    #endif // TEST_RUN

                    if( a.finishes() )
                    {
                        m += "\n\t <-- And finish.\n";
                        reprint(game);
                        cout << m << endl;
                        #ifndef TEST_RUN

                        wait(PROCRAST, .25);
                        #endif // TEST_RUN
                        game.finishTurn();
                    }
                    else
                    {
                        m += "\n\t => And go all in.\n";
                        reprint(game);
                        cout << m << endl;
                        game.roll();
                        #ifndef TEST_RUN
                        wait(PROCRAST, .5);

                        #endif // TEST_RUN
                    }
                }
                else
                {
                    m += "\n\t - welp -";
                    reprint(game);
                    cout << m << endl;
                    game.finishTurn();
                }

            }   // bot moves
        }
        catch(illegal_move_error& err)
        {
            if( game.getPlayer() == 1 )
            {
                cout << "Invalid." << endl;

                #ifndef TEST_RUN
                wait(PROCRAST, .5);
                #endif // TEST_RUN
            }

            s += std::string(" | ") + err.what();

        }
        catch ( ... )
        {
            cout << "what happended???" << endl;
            exit(43);
        }
    }

    cleanup:
    delete bot;

    return 0;
}

void gradDesc_hyperparameter()
{
    static constexpr int T   =  100000;
    static constexpr int N   = 10000;
    static constexpr double d = 0.03;
    static constexpr double doubleAlpha = 0.003;

    using Game_t = Sarsa::Game_t;

//    double a = 0.9;
//    double e = 0.9;
//    double g = 0.1;
//    double l = 0.1;

    double a = 0.5;
    double e = 0.5;
    double g = 0.5;
    double l = 1;

    auto c = [](double& v){v=v>1?1:v<0?0:v;};

    while(true)
    {
        double goal = evalParams(a,e,g,l,T,N);
        cout << "\t( " << goal << " )" << endl;

        double na = a+d;    // new a
        double ne = e+d;
        double ng = g+d;
        double nl = l+d;
        c(na);
        c(ne);
        c(ng);
        c(nl);

        double da = evalParams(na,e, g, l,T,N) - goal;   // wie viele punkte man dadurch erhält
        double de = evalParams(a, ne,g, l,T,N) - goal;
        double dg = evalParams(a, e,ng, l,T,N) - goal;
        double dl = evalParams(a, e, g,nl,T,N) - goal;

        a = a+doubleAlpha*da;
        e = e+doubleAlpha*de;
        g = g+doubleAlpha*dg;
        l = l+doubleAlpha*dl;
        c(a);
        c(e);
        c(g);
        c(l);

//        cout << "Corrected to: " << endl;
        cout <<   "a == " << std::fixed << std::setprecision(2) << a
             << "\te == " << std::fixed << std::setprecision(2) << e
             << "\tg == " << std::fixed << std::setprecision(2) << g
             << "\tl == " << std::fixed << std::setprecision(2) << l;




    } // while true

}

double evalParams(double a,double e,double g,double l,int T, int N)
{
    using Game_t = Sarsa::Game_t;

    double alph = a;
    double eps = e;
    double gam = g;
    double lamb = l;
    Game_t* env;
    Sarsa*  bot;


    // : training
//    cout << "\ttrain..." << endl;
    env = new Game_t();
    bot = new Sarsa(env, alph, eps, gam, lamb);
    for( int i = 0; i < T; ++i)
        bot->performEpisode();

    // : evaluate
//    cout << "\teval..." << endl;
    env->init();
    unsigned int total = 0;
    for(int i = 1; i <= N; ++i)
    {
        auto s = env->cloneState();
        auto& corr = dynamic_cast<Game_t::State_t&>(*s);
        Game_t::State_t copy(corr);
        std::shared_ptr<Game_t::State_t> state( new Game_t::State_t(copy) );
        Sarsa::Action_t a = bot->greedy( *state, false );

        if( copy.isTerminal() )
        {
            // (this case increments i by 1 (through for loop) and resets state)
            //
            env->init();
            continue;
        }

        try
        {
            env->takeAction( a );
        }
        catch(illegal_move_error& err)
        {
            /// \note this cannot happen because env->takeAction does not throw
            env->init();
            continue;
        }
        if( a.finishes() )
        {
            total += env->getPoints(0);
            env->init();
            continue;
        }
        --i;    // game continues -- but we want evaluationTime EPISODES, not moves
    }   // test episodes
    double averageScore = ( (double)total ) / N;

//    cout << "\t" << a << " " << e << " " << g << " " << l << " | " << averageScore << endl;

    delete env;
    delete bot;

    return averageScore;
}

