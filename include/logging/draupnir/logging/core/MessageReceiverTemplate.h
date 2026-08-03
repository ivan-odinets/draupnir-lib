#ifndef MESSAGERECEIVERTEMPLATE_H
#define MESSAGERECEIVERTEMPLATE_H

#include "draupnir/logging/core/AbstractMessageReceiver.h"

#include "draupnir/messages/categories/MessageCategoriesSerializerInterface.h"
#include "draupnir/messages/concepts/MessageCategoryTraitConcept.h"
#include "draupnir/messages/models/MessageListModel.h"

namespace Draupnir::Logging
{

template<Draupnir::Messages::MessageCategoryTraitConcept... MessageCategoryTraits>
class MessageReceiverTemplate :
    public AbstractMessageReceiver,
    public Draupnir::Messages::MessageCategoriesSerializerInterface
{
public:
    using SingleCategorySerializer = ::Draupnir::Settings::FlagSerializerTemplate<
        Draupnir::Messages::MessageCategory,
        MessageCategoryTraits...
    >;

    using Serializer = ::Draupnir::Settings::FlagsMaskSerializerTemplate<
        Draupnir::Messages::MessageCategories,
        SingleCategorySerializer,
        ::Draupnir::Settings::NoneFlagsMaskTemplate<Draupnir::Messages::MessageCategories>,
        ::Draupnir::Settings::FlagsMaskWrapperTemplate<
            []() { return Draupnir::Messages::MessageCategories::All; }, []() { return Draupnir::Messages::MessageCategories::allConfigKey(); }>
        >;

    std::optional<Draupnir::Messages::MessageCategories> fromConfigString(const QString &configString) const final {
        return Serializer::fromConfigString(configString);
    }

    QString toConfigString(Draupnir::Messages::MessageCategories mask) const final {
        return Serializer::toConfigString(mask);
    }

    Draupnir::Messages::MessageListModel* messages() { return &m_messageListModel; }

    /*! @brief Handles a single @ref Draupnir::Messages::MessagePtr object.
     *  @param message @ref Draupnir::Messages::MessagePtr referencing the message object to be handled. */
    void handleMessage(const Draupnir::Messages::MessagePtr& message) override {
        m_messageListModel.append(message);
    }

    /*! @brief Handles a @ref Draupnir::Messages::MessageList.
     *  @param messageList @ref Draupnir::Messages::MessageList object containing @ref Draupnir::Messages::MessagePtr
     *         objects to be handled. */
    void handleMessageList(const Draupnir::Messages::MessageList& messageList) override {
        m_messageListModel.append(messageList);
    };

private:
    Draupnir::Messages::MessageListModel m_messageListModel;
};

};

#endif // MESSAGERECEIVERTEMPLATE_H
