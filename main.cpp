#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "move_monster.h"
#include "funkcje.h"
#include "postac.h"
#include "bohater.h"
#include "obiekt.h"
#include "potwor.h"
#include "dzwiek.h"
#include "Struct_promien_slyszenia.h"
#include "TextureManager.h"
#include "pokoj.h"
#include "menu.h"


using namespace std;
using namespace sf;

vector<Time> loadScores()
{
    vector<Time> scores;
    ifstream file("scores.txt");
    string line;
    while (getline(file, line))
    {
        float seconds = stof(line);
        scores.push_back(sf::seconds(seconds));
    }
    sort(scores.begin(), scores.end());
    return scores;
}

void saveScore(Time time)
{
    vector<Time> scores = loadScores();
    scores.push_back(time);
    sort(scores.begin(), scores.end());

    ofstream file("scores.txt");
    for (const auto& score : scores)
    {
        file << score.asSeconds() << "\n";
    }
}

int main()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "Horror Game", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        Menu menu(&window);

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                menu.handleInput(event);

                if (event.type == sf::Event::Closed ||
                    (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                    return 0;

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                {
                    int selected = menu.getSelectedItem();
                    if (selected == 1)
                        goto start_game;
                    else if (selected == 2)
                        return 0;
                }

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f clickPos(
                        static_cast<float>(event.mouseButton.x),
                        static_cast<float>(event.mouseButton.y)
                    );
                    if (menu.getOption(0).getGlobalBounds().contains(clickPos))
                    {
                        goto start_game;
                    }
                    else if (menu.getOption(1).getGlobalBounds().contains(clickPos))
                    {
                        return 0;
                    }

                }
            }

            menu.draw();
        }


    start_game:
    Clock gameClock;
    Time gameStartTime = gameClock.getElapsedTime();
    srand(time(NULL));
    bool develop_mode=false; //tryb "deweloperski" wylacza np. mgle wojny tak aby bylo widac co sie dzieje
    vector<Sprite*> to_draw; // tu jest mapa, sciany itd.
    vector<Postac*> postacie; // bohater
    vector<unique_ptr<obiekt>> obiekty; //tu są katy dostępu
    vector<unique_ptr<obiekt>> obiekty_do_chowania; //tu sa obiekty typu szafy i stoły w których można się chować
    vector <unique_ptr<promien_slysz>> promienie_sluchu; //tu sa przechowywane dzwieki tupniecia bohatera
    vector<unique_ptr<potwor>> potwory; //potwor

    Dzwiek dzwiek;
    dzwiek.load_serce_sound("assets/bohater/heartbeat.wav");
    dzwiek.start_serce_system();

    dzwiek.load_background_music("assets/bohater/ambience.wav");
    dzwiek.start_background_music();

    dzwiek.load_krzyk_sound("assets/bohater/krzyk.wav");


    Clock clock;
    ////////window///////////////

    Vector2u windowSize = window.getSize();

    const float baseX = 1920.f;
    const float baseY = 1080.f;
    const float window_X = static_cast<float>(windowSize.x);
    const float window_Y = static_cast<float>(windowSize.y);

    const float Scale_ratioX = window_X / baseX;
    const float Scale_ratioY = window_Y / baseY;
    const float Scale_general=(Scale_ratioX+Scale_ratioY)/2.f;

    window.setFramerateLimit(60);
    if (develop_mode) {cout << "Okno: " << window_X << " x " << window_Y << endl; cout << "Skala X: " << Scale_ratioX << " Skala Y: " << Scale_ratioY << endl;}

    ////////plansza końcowa/////////////
    unique_ptr<Sprite> end_win = make_unique<Sprite>();
    end_win->setTexture(TextureManager::getInstance().getTexture("assets\\ekran_koncowy_wygrana.png"));
    end_win->setScale(1,1);
    end_win->setTextureRect(IntRect(0, 0, windowSize.x, windowSize.y));
    set_proper_scale(end_win, Scale_ratioX,Scale_ratioY);

    unique_ptr<Sprite> end_lose = make_unique<Sprite>();
    end_lose->setTexture(TextureManager::getInstance().getTexture("assets\\ekran_koncowy.png"));
    end_lose->setScale(1,1);
    end_lose->setTextureRect(IntRect(0, 0, windowSize.x, windowSize.y));
    set_proper_scale(end_lose, Scale_ratioX,Scale_ratioY);


    //////////////// background/////////////////////////////
    unique_ptr<Sprite> background = make_unique<Sprite>();
    Texture& bgTexture = TextureManager::getInstance().getTexture("assets\\mapa\\tlo.png");
    bgTexture.setRepeated(true);
    background->setTexture(bgTexture);
    background->setScale(1,1);
    background->setTextureRect(IntRect(0, 0, windowSize.x, windowSize.y));
    set_proper_scale(background, Scale_ratioX,Scale_ratioY);
    to_draw.push_back(background.get());
    /////////////////////////////////////////////////
    /////floor/////////////////////////
    unique_ptr<Sprite> floor = make_unique<Sprite>();
    Texture& flTexture = TextureManager::getInstance().getTexture("assets\\mapa\\podloga.png");
    floor->setTexture(flTexture);
    floor->setPosition(windowSize.x/2,windowSize.y/2);
    floor->setScale(1,1);
    set_proper_scale(floor, Scale_ratioX,Scale_ratioY);
    reset_origin_point(floor);
    to_draw.push_back(floor.get());
    ////////////////////////////////////////////////
    ///////////sciany///////////////////////
    Image image_sciany;
    image_sciany.loadFromFile("assets\\mapa\\sciany.png"); //potrzebne do kolizji

    auto sciany = make_unique<Sprite>();
    Texture& scTexture = TextureManager::getInstance().getTexture("assets\\mapa\\sciany.png");
    sciany->setTexture(scTexture);
    sciany->setPosition(windowSize.x/2,windowSize.y/2);
    sciany->setScale(1,1);
    set_proper_scale(sciany, Scale_ratioX,Scale_ratioY);
    reset_origin_point(sciany);
    to_draw.push_back(sciany.get());
    //pozycje opisane za pomocą "kratek" (na potrzeby liczenia ścieżki)
    vector<floor_square*> floor_tile=create_floor(image_sciany,Scale_ratioX,Scale_ratioY);
    floor_square* hero_pos=nullptr;
    floor_square* goal=nullptr;

    floor_square* room_goal=nullptr;
    floor_square* monster_pos=nullptr;


    vector<floor_square*> path; // tu jest seria "kafelków" które ma odwiedzić potwór po drodze do obranego celu

    const int number_of_rooms=12; //z góry ustalana liczba pokoji które sa na mapie
    vector<room> rooms=create_room();
    if (rooms.empty()) {
        cerr << "Blad: brak pokoi!" << endl;
        return -1;
    }
    ///////////////////////////////////
    ////////OBIEKTY/////////////////////
    unique_ptr<obiekt> drzwi = make_unique<obiekt>();
    Texture& drzwi_zam=TextureManager::getInstance().getTexture("assets\\obiekty\\drzwi_zam.png");
    Texture&drzwi_otw=TextureManager::getInstance().getTexture("assets\\obiekty\\drzwi_otw.png");
    drzwi->setTexture(drzwi_zam);
    drzwi->setScale(0.95f,0.95f);
    set_proper_scale(drzwi, Scale_ratioX,Scale_ratioY);
    reset_origin_point(drzwi);
    drzwi->setPosition(windowSize.x/1.2f, windowSize.y/3.1f);
    obiekty.push_back(std::move(drzwi));

    unique_ptr<obiekt> terminal = make_unique<obiekt>();
    terminal->setTexture(TextureManager::getInstance().getTexture("assets\\obiekty\\terminal.png"));
    terminal->setScale(0.05f,0.05f);
    set_proper_scale(terminal, Scale_ratioX,Scale_ratioY);
    terminal->setPosition(windowSize.x/1.15f, windowSize.y/3.f);
    reset_origin_point(terminal);
    obiekty.push_back(std::move(terminal));


    unique_ptr<obiekt> karta1 = make_unique<obiekt>();
    karta1->setTexture(TextureManager::getInstance().getTexture("assets\\obiekty\\karta1.png"));
    unique_ptr<obiekt> karta2 = make_unique<obiekt>();
    karta2->setTexture(TextureManager::getInstance().getTexture("assets\\obiekty\\karta2.png"));
    unique_ptr<obiekt> karta3 = make_unique<obiekt>();
    karta3->setTexture(TextureManager::getInstance().getTexture("assets\\obiekty\\karta3.png"));

    karta1->set_can_pick_up(true);
    karta2->set_can_pick_up(true);
    karta3->set_can_pick_up(true);

    karta1->setScale(0.02f,0.02f);
    karta2->setScale(0.02f,0.02f);
    karta3->setScale(0.02f,0.02f);
    set_proper_scale(karta1, Scale_ratioX,Scale_ratioY);
    set_proper_scale(karta2, Scale_ratioX,Scale_ratioY);
    set_proper_scale(karta3, Scale_ratioX,Scale_ratioY);

    //położenie kart jest losowe (1 z 3 możliwych)
    const int polozenie_kart=rand()%3+1;
    switch(polozenie_kart){
        case 1:{
        karta1->setPosition(windowSize.x/5.3f, windowSize.y/8.4f);
        karta2->setPosition(windowSize.x/2.3f, windowSize.y/1.35f);
        karta3->setPosition(windowSize.x/1.425f, windowSize.y/7.f);
        break;
        }
        case 2:{
            karta1->setPosition(windowSize.x/3.35f, windowSize.y/8.6f);
            karta2->setPosition(windowSize.x/1.7f, windowSize.y/2.5f);
            karta3->setPosition(windowSize.x/4.9f, windowSize.y/1.2f);
            break;
        }
        case 3:{
            karta1->setPosition(windowSize.x/9.f, windowSize.y/1.66f);
            karta2->setPosition(windowSize.x/2.3f, windowSize.y/3.3f);
            karta3->setPosition(windowSize.x/1.7f, windowSize.y/2.5f);
            break;
        }
    }

    obiekty.push_back(std::move(karta1));
    obiekty.push_back(std::move(karta2));
    obiekty.push_back(std::move(karta3));
    ////OBIEKTY DO CHOWANIA DLA BOHATERA//////
    unique_ptr<obiekt> szafa1 = make_unique<obiekt>();
    szafa1->setTexture(TextureManager::getInstance().getTexture("assets\\obiekty\\szafa.png"));
    szafa1->setScale(0.95f,0.95f);
    set_proper_scale(szafa1, Scale_ratioX,Scale_ratioY);
    reset_origin_point(szafa1);
    szafa1->setPosition(windowSize.x/2.24f, windowSize.y/2.f);
    szafa1->set_can_hide_in(true);
    obiekty_do_chowania.push_back(std::move(szafa1));

    unique_ptr<obiekt> szafa2 = make_unique<obiekt>();
    szafa2->setTexture(TextureManager::getInstance().getTexture("assets\\obiekty\\szafa.png"));
    szafa2->setScale(0.95f,0.95f);
    set_proper_scale(szafa2, Scale_ratioX,Scale_ratioY);
    reset_origin_point(szafa2);
    szafa2->setPosition(windowSize.x/5.f, windowSize.y/1.4f);
    szafa2->set_can_hide_in(true);
    obiekty_do_chowania.push_back(std::move(szafa2));

    unique_ptr<obiekt> szafa3 = make_unique<obiekt>();
    szafa3->setTexture(TextureManager::getInstance().getTexture("assets\\obiekty\\szafa.png"));
    szafa3->setScale(0.95f,0.95f);
    set_proper_scale(szafa3, Scale_ratioX,Scale_ratioY);
    reset_origin_point(szafa3);
    szafa3->setPosition(windowSize.x/1.67f, windowSize.y/3.1f);
    szafa3->set_can_hide_in(true);
    obiekty_do_chowania.push_back(std::move(szafa3));
    ////////////////////////////////////////

    ////////////////////////////////////
    ///////////Potwor//////////////////
    unique_ptr<potwor>monster = make_unique<potwor>();
    monster->setPosition(windowSize.x/1.18f,windowSize.y/2.f);
    monster->set_v_ratio(1.f);
    monster->set_x_speed(100.f);
    monster->set_y_speed(100.f);
    monster->setScale(0.7f,0.7f);
    monster->reset_origin_point();
    set_proper_scale(monster,Scale_ratioX,Scale_ratioY);
    postacie.push_back(monster.get());
    potwory.push_back(std::move(monster));


    unique_ptr<ConvexShape> cone = make_unique<ConvexShape>(); //stożek widoczności potwora używany tylko podczas trybu developerskiego (dla lepszej wizualizacji działania systemu widzenia)
    cone->setFillColor(sf::Color(255, 255, 0, 80));

    /////////////////////////////////////////////////
    ////////////////// bohater /////////////////////
    unique_ptr<bohater>hero = make_unique<bohater>();
    hero->setPosition(windowSize.x/6.f,windowSize.y/2.f);
    hero->setScale(1.2f,1.2f);
    hero->set_v_ratio(1.f);
    hero->set_x_speed(120.f);
    hero->set_y_speed(120.f);
    hero->reset_origin_point();
    set_proper_scale(hero,Scale_ratioX,Scale_ratioY);
    postacie.push_back(hero.get());
    dzwiek.load_chodzenie_sound("assets/bohater/ruch.wav");
    ///////////////////////////////////////
    ////"mgła wojny" (czarna otoczka wokół bohatera)
    Shader fog_of_war;
    if (!fog_of_war.loadFromFile("assets\\bohater\\mask.frag", Shader::Fragment)) {
        cout << "Nie udalo sie zaladowac shadera!" << endl;
    }
    fog_of_war.setUniform("lightCenter", Glsl::Vec2(hero->getPosition()));
    fog_of_war.setUniform("lightRadius", 300.f);
    fog_of_war.setUniform("resolution", Glsl::Vec2(window.getSize()));

    float aktualny_promien=200.f;
    RectangleShape mask(Vector2f(windowSize.x, windowSize.y));
    mask.setFillColor(Color::Black);
    /////////////////////////

    ////////////////////////////////////////////////
    int frame_count1=0, frame_count2=0, frame_count_h=0, frame_count_m=0; //frame counter bohatera i potwora (używane do zmiany klatek animacji )
    Time czas_do_nowego_promienia = Time::Zero;
    Time TimeP = Time::Zero; //liczenie czasu od momentu osiągnięcia pokoju (3 sekundy i idzie dalej)
    Time TimeZ = Time::Zero; //liczenie czasu od momentu zobaczenia/usłyszenia bohatera (jak dojdzie do np. źródła dźwięku czeka sekunde i idzie dalej)
    bool czy_pokoj_wybrany=false;
    bool koniec_wygrana=false;
    bool koniec_przegrana = false;
    bool czy_drzwi_otwarte=false;
    bool can_use_e = true;
    bool previous_can_see = false;

    bool game_running = true;
    while (window.isOpen() && game_running)  {
        Time elapsed=clock.restart();
        Event event;


        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                window.close();
                return 0;
            }
        }
        window.clear(Color::Black);

        ////////////ruszanie///////////
        if (!hero->get_is_hidden()){
            move_hero(hero, elapsed, Scale_ratioX, Scale_ratioY, image_sciany, promienie_sluchu,czas_do_nowego_promienia,dzwiek,potwory);
        }
        else
        {
            dzwiek.stop_chodzenie();
        }

        float distance_to_monster = distance_between(potwory[0], hero);
        dzwiek.update_serce_heartbeat(distance_to_monster);

        //fog_of_war->setPosition(hero->getPosition());
        fog_of_war.setUniform("lightCenter", hero->getPosition());
        fog_of_war.setUniform("lightRadius", aktualny_promien);


        ///zmienianie klatek animacji bohatera
        const int kl_h=10;
        if ((frame_count1%kl_h)+1==kl_h){
            frame_count_h++;
            hero->change_frame(frame_count_h);
        }
        /////podnoszenie kart ///////////////
        if (hero->get_inventory().size()!=3){ //jak już są 3 karty w ekwipunku to znaczy że są już wszystkie i nie trzeba znowu sprawdzać
                for (auto it = obiekty.begin(); it != obiekty.end(); ) {
                    if ((*it)->get_can_pick_up() && (*it)->getGlobalBounds().intersects(hero->getGlobalBounds())) {

                        hero->add_item(std::move(*it));
                        it = obiekty.erase(it);
                    } else {
                        ++it;
                    }
            }
        }
        if (!Keyboard::isKeyPressed(Keyboard::E)) {
            can_use_e = true;
        }

        // WYJŚCIE z obiektów do chowania
        if (hero->get_is_hidden() && Keyboard::isKeyPressed(Keyboard::E) && can_use_e){
            hero->set_is_hidden(false);
            can_use_e = false;
        }

        // WEJŚCIE z obiektów do chowania
        for (auto &t : obiekty_do_chowania){
            if (!hero->get_is_hidden() && distance_between_m(t,hero)<50.f*Scale_general && Keyboard::isKeyPressed(Keyboard::E) && can_use_e){
                hero->set_is_hidden(true);
                dzwiek.stop_chodzenie();
                can_use_e = false;
            }
        }


        /////////////////////otwarcie drzwi/////////////
        if (distance_between_m(obiekty[1], hero) < 70.f*Scale_general)
        {
            if (hero->get_inventory().size()>=2)
            {
                obiekty[0]->setTexture(drzwi_otw);
                obiekty[0]->setScale(0.95f*Scale_general,0.95f*Scale_general);
                reset_origin_point(obiekty[0]);
                obiekty[0]->setPosition(windowSize.x/1.2f, windowSize.y/3.1f);
                czy_drzwi_otwarte=true;
            }
        }
        //sprawdzenie czy bohater jest blisko drzwi i czy są otwarte
        if (distance_between_m(obiekty[0], hero) < 85.f*Scale_general) {
            if (czy_drzwi_otwarte)
            {
                dzwiek.stop_chodzenie();
                window.clear(Color::Black);
                window.draw(*end_win);
                window.display();
                Time gameEndTime = gameClock.getElapsedTime();
                Time totalTime = gameEndTime - gameStartTime;
                saveScore(totalTime);
                sleep(seconds(5));
                game_running = false;
                break;
            }
        }

        ///aktualizacja pozycji na "kratownicy"
        for (auto &t : floor_tile){
            if (t->get_is_wall()) {
                t->setFillColor(Color(255, 0, 0, 200));  // czerwony
            } else {
                t->setFillColor(Color(200, 200, 200, 128));  // szary
            }
        }
        for (auto &t : floor_tile){
            if (t->getGlobalBounds().contains(hero->getPosition())) {
                hero_pos = t;
                break;
            }
        }
        for (auto &t : floor_tile){
            if (t->getGlobalBounds().contains(potwory[0]->getPosition())) {
                monster_pos = t;
                break;
            }
        }


        //////////////caly ruch potwora//////////////////////////////////////////////////////////////
        bool can_see = check_if_hero_visible(potwory[0],hero,image_sciany,Scale_general,cone);
        bool can_hear = check_if_hero_hearable(promienie_sluchu,potwory[0]);

        dzwiek.update_krzyk_status();

        if (can_see && !hero->get_is_hidden() && !previous_can_see) {
            dzwiek.play_krzyk();
        }

        previous_can_see = can_see && !hero->get_is_hidden();

        potwory[0]->set_v_ratio(1.f);



        if (can_see && !hero->get_is_hidden()) { //jeżeli widzi bohatera to biegnie bezpośrednio do niego
            potwory[0]->set_v_ratio(potwory[0]->get_v_ratio() * 2.f);
            goal = hero_pos;  // Cel = pozycja bohatera
            room_goal=nullptr;
        }
        // Jeśli nie widzi, ale słyszy
        else if (!promienie_sluchu.empty() && can_hear) { // jeżeli słyszy bohatera to biegnie do źródła dźwięku
            potwory[0]->set_v_ratio(potwory[0]->get_v_ratio() * 2.f);
            for (auto &t : floor_tile) {
                if (t->getGlobalBounds().contains(promienie_sluchu[0]->getPosition())) {
                    goal=t;
                }
            }
            room_goal=nullptr;
        }
        // Jeśli nie widzi i nie słyszy, ale ma już cel (np. ostatnią znaną pozycję bohatera)
        else if (goal != nullptr) {
            potwory[0]->set_v_ratio(1.f);  // Normalna prędkość
        }
        else {
            potwory[0]->set_v_ratio(1.f);
            goal = nullptr;  // Brak celu
        }

        if (hero->get_is_hidden() && goal!=hero_pos && path.empty()) {
            goal = nullptr;
            czy_pokoj_wybrany = false;
            can_see=false;
        }

        if (!can_see && !can_hear && !czy_pokoj_wybrany) { //jeżeli nie widzi ani nie słyszy bohatera to potwór biega po pokojach wybierając losowy i losowe miejsce w nim
            TimeP = Time::Zero;
            const int room_nr = rand() % number_of_rooms;
            int attempts = 0;
            const int max_attempts = 100;
            bool czy_znalazl = false;

            while (!czy_znalazl && attempts < max_attempts) {
                attempts++;
                int x = rooms[room_nr].xl + rand() % (rooms[room_nr].xr - rooms[room_nr].xl + 1);
                int y = rooms[room_nr].yu + rand() % (rooms[room_nr].yd - rooms[room_nr].yu + 1);

                for (auto &t : floor_tile) {
                    if (t->get_x() == x && t->get_y() == y && !t->get_is_wall()) {
                        czy_pokoj_wybrany = true;
                        room_goal = t;
                        goal=nullptr;
                        czy_znalazl = true;
                        break;
                    }
                }

            }
        }

        if (goal) { //jeżeli celem jest coś związanego z bohaterem
                for (auto &tile : floor_tile) {
                    tile->reset_astar_state();
                }
            if (!(distance_between_p(monster_pos, goal) < 30.f*Scale_general)) { //jeżeli dobiegł do np. źródła dźwięku to chwile w nim postoji i ruszy dalej
                path = create_path(goal, monster_pos);
                TimeZ = Time::Zero;
                  // reset czekania, bo jeszcze nie dotarł
            } else {
                TimeZ += elapsed;
                potwory[0]->set_is_running(false);
                    if (TimeZ > seconds(1.5f)) {
                        goal = nullptr;
                        czy_pokoj_wybrany = false;
                        TimeZ = Time::Zero;
                    }

            }
        }
        else if (room_goal){ //jeżeli celem jest kolejny pokój
                for (auto &tile : floor_tile) {
                    tile->reset_astar_state();
                }

            if (!(distance_between_p(monster_pos,room_goal)<60.f*Scale_general) ){
                    path = create_path(room_goal, monster_pos);
            }
            else{
                TimeP+=elapsed;
                potwory[0]->set_is_running(false);
                if (TimeP>seconds(3)){
                    czy_pokoj_wybrany=false;
                    room_goal=nullptr;
                }
            }
        }
          else {
            path.clear();  // Brak ścieżki
        }

        const int kl_m=8; //zmiana klatek animacji potwora
        if ((frame_count2%kl_m)+1==kl_m){
            frame_count_m++;
            potwory[0]->change_frame(frame_count_m, Scale_ratioX, Scale_ratioY);
        }
        if (!path.empty()){
            move_monster(potwory[0],path,elapsed,Scale_ratioX,Scale_ratioY);
        }
        if (potwory[0]->getGlobalBounds().intersects(hero->getGlobalBounds()) && !hero->get_is_hidden())
        {
            dzwiek.stop_chodzenie();
            dzwiek.stop_background_music();
            window.clear(Color::Black);
            window.draw(*end_lose);
            window.display();
            sleep(seconds(5));
            game_running = false;
            break;
        }
        //////////////////////////////////////////////////////////////////////////
        ///////////////////////////////
        ///////////DRAWING/////////////
        if (!koniec_wygrana&& !koniec_przegrana){
            for (auto &d : to_draw){
                window.draw(*d);
            }
            for (auto &t : obiekty ) {
                window.draw(*t);
            }
            for (auto &t : obiekty_do_chowania ) {
                window.draw(*t);
            }
            for (auto &d : postacie){
                if (d->get_is_hidden()) {continue;}
                window.draw(*d);
            }
            if (!develop_mode){
                window.draw(mask, &fog_of_war);
            }
        }

        for (auto& p : promienie_sluchu) {
            if (p->get_pozostaly_czas() >= milliseconds(0)) {
                p->set_pozostaly_czas(p->get_pozostaly_czas() - elapsed);
                p->setRadius(p->getRadius() + 5.f);
                reset_origin_point(p);

            }
        }
        promienie_sluchu.erase(remove_if(
            promienie_sluchu.begin(), promienie_sluchu.end(), [](const unique_ptr<promien_slysz>& p) {
                return p->get_pozostaly_czas() < seconds(0.05);}), promienie_sluchu.end());


        if (!develop_mode){
            window.draw(mask, &fog_of_war);
        }
        if (develop_mode) {
            window.draw(*cone);
            for (auto& p : promienie_sluchu){
                window.draw(*p);
            }

            //////////rysowanie podglogi//////////

            for (auto &t : floor_tile){
                window.draw(*t);
            }

            for (auto &t : path) {
                window.draw(*t);
            }
            if (hero_pos) {
                hero_pos->setFillColor(Color(255, 255, 0, 128));
                window.draw(*hero_pos);
            }
            if (monster_pos) {
                monster_pos->setFillColor(Color(0, 255, 0, 128));
                window.draw(*monster_pos);
            }
        }
        window.display();
        frame_count1++;
        frame_count2++;
        czas_do_nowego_promienia -= elapsed;

        if (potwory.empty())
        {
            game_running = false;
            break;
        }

    }


    for (floor_square* ptr : floor_tile)
    {
        delete ptr;
    }


    if (!window.isOpen())
    {
        break;
    }

}

return 0;
}

