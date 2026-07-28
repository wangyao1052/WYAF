///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2026 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYDB_CHAIN_UPDATE_FEEDBACK_H
#define WYDB_CHAIN_UPDATE_FEEDBACK_H

#include <string>
#include <memory>

#include <wydbDefs.h>
#include <wydbElementId.h>
#include <wydbElement.h>

NS_WYDB_BEG

// Chain update feedback interface.
//
// This interface describes a feedback item produced during chain update.
// A feedback item can be used to report the resulting health state of an
// element, along with a caller-defined code and message.
class ChainUpdateFeedback
{
public:
    ChainUpdateFeedback() = default;
    virtual ~ChainUpdateFeedback() = default;
    ChainUpdateFeedback(const ChainUpdateFeedback&) = delete;
    ChainUpdateFeedback& operator=(const ChainUpdateFeedback&) = delete;
    ChainUpdateFeedback(ChainUpdateFeedback&&) = delete;
    ChainUpdateFeedback& operator=(ChainUpdateFeedback&&) = delete;

    // Get the id of the associated element.
    virtual ElementId getElementId() const = 0;

    // Get the health state indicated by this feedback.
    virtual wydb::Element::HealthState getHealthState() const = 0;

    // Get the caller-defined code that identifies this feedback type.
    virtual unsigned int getCode() const = 0;

    // Get the message associated with this feedback.
    virtual std::string getMessage() const = 0;
};

using ChainUpdateFeedbackSPtr = std::shared_ptr<ChainUpdateFeedback>;

// Chain update feedback collector.
// This interface collects feedback items produced during chain update.
class ChainUpdateFeedbackCollector
{
public:
    // Add a feedback item.
    virtual wy::ErrorStatus append(ChainUpdateFeedbackSPtr feedback) = 0;

protected:
    ChainUpdateFeedbackCollector() = default;
    virtual ~ChainUpdateFeedbackCollector() = default;
    ChainUpdateFeedbackCollector(const ChainUpdateFeedbackCollector&) = delete;
    ChainUpdateFeedbackCollector& operator=(const ChainUpdateFeedbackCollector&) = delete;
    ChainUpdateFeedbackCollector(ChainUpdateFeedbackCollector&&) = delete;
    ChainUpdateFeedbackCollector& operator=(ChainUpdateFeedbackCollector&&) = delete;
};

NS_WYDB_END

#endif // WYDB_CHAIN_UPDATE_FEEDBACK_H