#ifndef ABOUTDRAUPNIRLIBENTRYHANDLER_H
#define ABOUTDRAUPNIRLIBENTRYHANDLER_H

#include "handlers/AbstractHandlers.h"
#include "traits/entries/HelpMenuEntries.h"

#include <QMessageBox>

namespace Draupnir::Handlers
{

template<class Context,class HandledEntry>
class GenericMenuEntryHandler;

/*! @class GenericHelpMenuEntryHandler<HelpContext, Draupnir::Menus::AboutDraupnirLibMenuTrait>
 *  @headerfile draupnir-lib/include/templateHandlers/handlers/helpMenu/AboutDraupnirLibEntryHandler.h
 *  @brief Partial specialization that wires a QAction to the standard dialog About Draupnir Lib.
 *  @tparam HelpContext Any type representing the help/“About” handling context (unused in this specialization).
 *
 * @note The connection is made without an explicit QObject context; lifetime of QAction must be managed externally. */

template<class HelpContext>
class GenericMenuEntryHandler<HelpContext,Draupnir::Menus::AboutDraupnirLibMenuTrait> :
        public ActionHandler<GenericMenuEntryHandler<HelpContext,Draupnir::Menus::AboutDraupnirLibMenuTrait>,Draupnir::Menus::AboutDraupnirLibMenuTrait>
{
public:
    /*! @brief Constructs the handler.
     *  @param HelpContext Reference to a help context. Not used by this specialization. */
    GenericMenuEntryHandler(HelpContext&) {}

    /*! @brief This method will be executed when user clicks on the About Draupnir Lib QAction.
     * @todo Write a bit more extended text. */
    void onTriggered() {
        QMessageBox::information(
            nullptr,
            QObject::tr("About Draupnit Lib"),
            QObject::tr("This Application is using the DraupnirLib.");
        );
    }
};

} // namespace Draupnir::Menus

#endif // ABOUTDRAUPNIRLIBENTRYHANDLER_H
