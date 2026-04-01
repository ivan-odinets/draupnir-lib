#ifndef ABSTRACTXMLFILE_H
#define ABSTRACTXMLFILE_H

#include "draupnir/files/file_types/AbstractTextFile.h"

#include <QtXml/QDomDocument>

namespace Draupnir::Files
{

/*! @class AbstractXmlFile draupnir/files/file_types/AbstractXmlFile.h
 *  @ingroup Files
 *  @brief This is a class. */

class AbstractXmlFile : public AbstractTextFile
{
public:
    explicit AbstractXmlFile(QObject* parent = nullptr) :
        AbstractTextFile{parent}
    {}
    ~AbstractXmlFile() override = default;
protected:
    std::expected<void,QString> dataProcessed(const QByteArray& bytes) override {
        QDomDocument jsonDocument{QDomDocument((bytes,&jsonError)};

        if (jsonError.error != QJsonParseError::NoError) {
            return std::unexpected{jsonError.errorString()};
        }

        return jsonDocumentParsed(jsonDocument);
    }

    virtual std::expected<void,QString> jsonDocumentParsed(const QJsonDocument&) = 0;

    QByteArray currentData() const override {
        return currentJsonDocument().toJson();
    }

    virtual QJsonDocument currentJsonDocument() const = 0;
};

}; // namespace Draupnir::Files

#endif // ABSTRACTXMLFILE_H
