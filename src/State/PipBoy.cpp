#include "../State/PipBoy.h"
#include "../Event/Keyboard.h"
#include "../Game/Game.h"
#include "../Game/Time.h"
#include "../Graphics/Renderer.h"
#include "../Input/Mouse.h"
#include "../ResourceManager.h"
#include "../UI/Factory/ImageButtonFactory.h"
#include "../UI/Image.h"
#include "../UI/ImageButton.h"
#include "../UI/MonthCounter.h"
#include "../UI/SmallCounter.h"

namespace Falltergeist
{
    using ImageButtonType = UI::Factory::ImageButtonFactory::Type;

    namespace State
    {
        PipBoy::PipBoy(std::shared_ptr<UI::IResourceManager> resourceManager) : State()
        {
            this->resourceManager = resourceManager;
            imageButtonFactory = std::make_unique<UI::Factory::ImageButtonFactory>(resourceManager);
        }

        PipBoy::~PipBoy()
        {
            Game::getInstance()->mouse()->popState();
        }

        void PipBoy::init()
        {
            if (_initialized) return;
            State::init();

            setModal(true);
            setFullscreen(true);

            Game::getInstance()->mouse()->pushState(Input::Mouse::Cursor::BIG_ARROW);

            // Background
            auto background = resourceManager->getImage("art/intrface/pip.frm");
            Point backgroundPos = Point((Game::getInstance()->renderer()->size() - background->size()) / 2);
            int backgroundX = backgroundPos.x();
            int backgroundY = backgroundPos.y();
            background->setPosition(backgroundPos);

            // Buttons
            auto alarmButton = imageButtonFactory->getByType(ImageButtonType::PIPBOY_ALARM_BUTTON, {backgroundX + 124, backgroundY + 13});
            auto statusButton = imageButtonFactory->getByType(ImageButtonType::SMALL_RED_CIRCLE, {backgroundX + 53, backgroundY + 340});
            auto automapsButton = imageButtonFactory->getByType(ImageButtonType::SMALL_RED_CIRCLE, {backgroundX + 53, backgroundY + 394});
            auto archivesButton = imageButtonFactory->getByType(ImageButtonType::SMALL_RED_CIRCLE, {backgroundX + 53, backgroundY + 423});
            auto closeButton = imageButtonFactory->getByType(ImageButtonType::SMALL_RED_CIRCLE, {backgroundX + 53, backgroundY + 448});
            closeButton->mouseClickHandler().add(std::bind(&PipBoy::onCloseButtonClick, this, std::placeholders::_1));
            // Date and time

            // Date
            auto gameTime = Game::getInstance()->gameTime();

            auto day = new UI::SmallCounter(backgroundPos + Point(21, 17));
            day->setLength(2);
            day->setNumber(gameTime->day());
            day->setColor(UI::SmallCounter::Color::WHITE);
            day->setType(UI::SmallCounter::Type::UNSIGNED);

            auto month = new UI::MonthCounter(
                static_cast<UI::MonthCounter::Month>(gameTime->month()),
                backgroundPos + Point(46, 18)
            );

            auto year = new UI::SmallCounter(backgroundPos + Point(84, 17));
            year->setLength(4);
            year->setNumber(gameTime->year());
            year->setColor(UI::SmallCounter::Color::WHITE);
            year->setType(UI::SmallCounter::Type::UNSIGNED);

            // Time
            auto time = new UI::SmallCounter(backgroundPos + Point(160, 17));
            time->setLength(4);
            time->setNumber((gameTime->hours() * 100) + gameTime->minutes());
            time->setColor(UI::SmallCounter::Color::WHITE);
            time->setType(UI::SmallCounter::Type::UNSIGNED);

            addUI(background);

            addUI(alarmButton);
            addUI(statusButton);
            addUI(automapsButton);
            addUI(archivesButton);

            addUI(day);
            addUI(month);
            addUI(year);
            addUI(time);

            addUI(closeButton);
        }

        void PipBoy::onCloseButtonClick(Event::Mouse* event)
        {
            Game::getInstance()->popState();
        }

        void PipBoy::onKeyDown(Event::Keyboard* event)
        {
            if (event->keyCode() == SDLK_ESCAPE)
            {
                Game::getInstance()->popState();
            }
        }
    }
}
