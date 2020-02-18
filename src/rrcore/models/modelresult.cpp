#include "modelresult.h"

ModelResult::ModelResult(const ModelResult &other) :
    m_code(other.code()),
    m_extra(other.extra())
{}

ModelResult::ModelResult(int code) :
    m_code(code)
{
}

ModelResult &ModelResult::operator=(const ModelResult &other)
{
    m_code = other.code();
    m_extra = other.extra();
    return *this;
}

int ModelResult::code() const
{
    return m_code;
}

QVariant ModelResult::extra() const
{
    return m_extra;
}

void ModelResult::setExtra(const QVariant &extra)
{
    m_extra = extra;
}

