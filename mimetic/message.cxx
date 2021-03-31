/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: message.cxx,v 1.4 2008-10-07 11:06:25 tat Exp $
 ***************************************************************************/
#include <fstream>
#include <cstdlib>
#include <mimetic/message.h>
#include <mimetic/contenttype.h>
#include <mimetic/utils.h>

namespace mimetic
{

using namespace std;
Attachment::Attachment(const std::string& fqn)
{
    set(fqn, ContentType("application","octet-stream"),Base64::Encoder());
}

Attachment::Attachment(const std::string& fqn, const ContentType& ct)
{
    set(fqn, ct, Base64::Encoder());
}

std::shared_ptr<Attachment> Attachment::create(const std::string& fqn) {
    return init(std::shared_ptr<Attachment>(new Attachment(fqn)));
}

std::shared_ptr<Attachment> Attachment::create(const std::string& fqn, const ContentType& ct) {
    return init(std::shared_ptr<Attachment>(new Attachment(fqn, ct)));
}

TextEntity::TextEntity()
{
    header().contentType("text/unknown");
}

TextEntity::TextEntity(const string& text)
{
    m_header.contentType("text/unknown");
    m_body.assign(text);
}

TextEntity::TextEntity(const string& text, const string& charset)
{
    ContentType ct("text", "unknown");
    ct.paramList().push_back(ContentType::Param("charset", charset));
    m_header.contentType(ct);
    m_body.assign(text);
}

std::shared_ptr<TextEntity> TextEntity::create() {
    return init(std::shared_ptr<TextEntity>(new TextEntity()));
}

std::shared_ptr<TextEntity> TextEntity::create(const string& text) {
    return init(std::shared_ptr<TextEntity>(new TextEntity(text)));
}

std::shared_ptr<TextEntity> TextEntity::create(const string& text, const string& charset) {
    return init(std::shared_ptr<TextEntity>(new TextEntity(text, charset)));
}

TextPlain::TextPlain(const string& text)
: TextEntity(text)
{
    m_header.contentType("text/plain");
}

TextPlain::TextPlain(const string& text, const string& charset)
: TextEntity(text,charset)
{
    m_header.contentType("text/plain");
}

std::shared_ptr<TextPlain> TextPlain::create(const std::string& text) {
    return init(std::shared_ptr<TextPlain>(new TextPlain(text)));
}

std::shared_ptr<TextPlain> TextPlain::create(const std::string& text, const string& charset) {
    return init(std::shared_ptr<TextPlain>(new TextPlain(text, charset)));
}

TextEnriched::TextEnriched(const string& text)
: TextEntity(text)
{
    m_header.contentType("text/enriched");
}
TextEnriched::TextEnriched(const string& text, const string& charset)
: TextEntity(text,charset)
{
    m_header.contentType("text/enriched");
}

std::shared_ptr<TextEnriched> TextEnriched::create(const std::string& text) {
    return init(std::shared_ptr<TextEnriched>(new TextEnriched(text)));
}

std::shared_ptr<TextEnriched> TextEnriched::create(const std::string& text, const string& charset) {
    return init(std::shared_ptr<TextEnriched>(new TextEnriched(text, charset)));
}

MultipartEntity::MultipartEntity()
{
    ContentType::Boundary boundary;
    ContentType ct("multipart", "unknown");
    ct.paramList().push_back(ContentType::Param("boundary", boundary));
    m_header.contentType(ct);
}

std::shared_ptr<MultipartEntity> MultipartEntity::create() {
    return init(std::shared_ptr<MultipartEntity>(new MultipartEntity()));
}

MultipartMixed::MultipartMixed()
{
    ContentType::Boundary boundary;
    ContentType ct("multipart", "mixed");
    ct.paramList().push_back(ContentType::Param("boundary", boundary));
    m_header.contentType(ct);
}

std::shared_ptr<MultipartMixed> MultipartMixed::create() {
    return init(std::shared_ptr<MultipartMixed>(new MultipartMixed()));
}

MultipartParallel::MultipartParallel()
{
    ContentType::Boundary boundary;
    ContentType ct("multipart", "parallel");
    ct.paramList().push_back(ContentType::Param("boundary", boundary));
    m_header.contentType(ct);
}

std::shared_ptr<MultipartParallel> MultipartParallel::create() {
    return init(std::shared_ptr<MultipartParallel>(new MultipartParallel()));
}

MultipartAlternative::MultipartAlternative()
{
    ContentType::Boundary boundary;
    ContentType ct("multipart", "alternative");
    ct.paramList().push_back(ContentType::Param("boundary", boundary));
    m_header.contentType(ct);
}

std::shared_ptr<MultipartAlternative> MultipartAlternative::create() {
    return init(std::shared_ptr<MultipartAlternative>(new MultipartAlternative()));
}

MultipartDigest::MultipartDigest()
{
    ContentType::Boundary boundary;
    ContentType ct("multipart", "digest");
    ct.paramList().push_back(ContentType::Param("boundary", boundary));
    m_header.contentType(ct);
}

std::shared_ptr<MultipartDigest> MultipartDigest::create() {
    return init(std::shared_ptr<MultipartDigest>(new MultipartDigest()));
}

ApplicationOctStream::ApplicationOctStream()
{
    m_header.contentType("application/octet-stream");
}

std::shared_ptr<ApplicationOctStream> ApplicationOctStream::create() {
    return init(std::shared_ptr<ApplicationOctStream>(new ApplicationOctStream()));
}

MessageRfc822::MessageRfc822(const std::shared_ptr<MimeEntity>& me)
: m_me(me)
{
    m_header.contentType("message/rfc822");
}

std::shared_ptr<MessageRfc822> MessageRfc822::create(const std::shared_ptr<MimeEntity>& me) {
    return init(std::shared_ptr<MessageRfc822>(new MessageRfc822(me)));
}

ostream& MessageRfc822::write(ostream& os,const char* eol) const
{
    MimeEntity::write(os);
    if (m_me != nullptr) {
        os << *m_me;
    }
    return os;
}

string ApplicationOctStream::type() const
{
    return m_header.contentType().param("type");
}

void ApplicationOctStream::type(const string& type)
{
    ContentType ct = m_header.contentType();
    ct.param("type",type);
    m_header.contentType(ct);
}

uint ApplicationOctStream::padding() const
{
    return utils::str2int(m_header.contentType().param("padding"));
}

void ApplicationOctStream::padding(unsigned int n)
{
    ContentType ct = m_header.contentType();
    ct.param("padding", utils::int2str(n));
    m_header.contentType(ct);
}

std::shared_ptr<ImageJpeg> ImageJpeg::create(const std::string& fqn)
{
    return init(std::shared_ptr<ImageJpeg>(new ImageJpeg(fqn)));
}

std::shared_ptr<AudioBasic> AudioBasic::create(const std::string& fqn)
{
    return init(std::shared_ptr<AudioBasic>(new AudioBasic(fqn)));
}

}
