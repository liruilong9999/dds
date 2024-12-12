#ifndef OMG_DDS_PUB_DATA_WRITER_IMPL_HPP_
#define OMG_DDS_PUB_DATA_WRITER_IMPL_HPP_

/* Copyright 2010, Object Management Group, Inc.
 * Copyright 2010, PrismTech, Corp.
 * Copyright 2010, Real-Time Innovations, Inc.
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */




/***************************************************************************
 *
 * dds/pub/DataWriter<> WRAPPER implementation.
 * Declaration can be found in dds/pub/DataWriter.hpp
 *
 ***************************************************************************/

#include <dds/topic/Topic.hpp>
#include <dds/pub/PublisherListener.hpp>
#include <dds/domain/DomainParticipantListener.hpp>
#include <org/eclipse/cyclonedds/core/ListenerDispatcher.hpp>

namespace dds
{
namespace pub
{

template <typename T, template <typename Q> class DELEGATE>
DataWriter<T, DELEGATE>::DataWriter(
    const dds::pub::Publisher& pub,
    const dds::topic::Topic<T>& topic) :
        dds::core::Reference< DELEGATE<T> >(
            new DELEGATE<T>(pub, topic, pub.default_datawriter_qos()))
{
    this->delegate()->listener(NULL, dds::core::status::StatusMask::none());
    this->delegate()->init(this->impl_);
}

template <typename T, template <typename Q> class DELEGATE>
DataWriter<T, DELEGATE>::DataWriter(const dds::pub::Publisher& pub,
     const ::dds::topic::Topic<T>& topic,
     const dds::pub::qos::DataWriterQos& qos,
     dds::pub::DataWriterListener<T>* listener,
     const dds::core::status::StatusMask& mask) :
         dds::core::Reference< DELEGATE<T> >(
            new DELEGATE<T>(pub, topic, qos))
{
    this->delegate()->listener(listener, mask);
    this->delegate()->init(this->impl_);
}

template <typename T, template <typename Q> class DELEGATE>
void
DataWriter<T, DELEGATE>::write(const T& sample)
{
    this->delegate()->write(sample);
}

template <typename T, template <typename Q> class DELEGATE>
void
DataWriter<T, DELEGATE>::write(const T& sample, const dds::core::Time& timestamp)
{
    this->delegate()->write(sample, timestamp);
}

template <typename T, template <typename Q> class DELEGATE>
void
DataWriter<T, DELEGATE>::write(const T& sample, const ::dds::core::InstanceHandle& instance)
{
    this->delegate()->write(sample, instance);
}

template <typename T, template <typename Q> class DELEGATE>
void
DataWriter<T, DELEGATE>::write(const T& sample,
        const ::dds::core::InstanceHandle& instance,
        const dds::core::Time& timestamp)
{
    this->delegate()->write(sample, instance, timestamp);
}

template <typename T, template <typename Q> class DELEGATE>
void
DataWriter<T, DELEGATE>::write(const dds::topic::TopicInstance<T>& i)
{
    this->delegate()->write(i);
}

template <typename T, template <typename Q> class DELEGATE>
void
DataWriter<T, DELEGATE>::write(const dds::topic::TopicInstance<T>& i,
        const dds::core::Time& timestamp)
{
    this->delegate()->write(i, timestamp);
}

template <typename T, template <typename Q> class DELEGATE>
template <typename FWIterator>
void
DataWriter<T, DELEGATE>::write(const FWIterator& begin, const FWIterator& end)
{
    FWIterator b = begin;
    while(b != end)
    {
        this->delegate()->write(*b);
        ++b;
    }
}

template <typename T, template <typename Q> class DELEGATE>
template <typename FWIterator>
void
DataWriter<T, DELEGATE>::write(const FWIterator& begin, const FWIterator& end,
        const dds::core::Time& timestamp)
{
    FWIterator b = begin;
    while(b != end)
    {
        this->delegate()->write(*b, timestamp);
        ++b;
    }
}

template <typename T, template <typename Q> class DELEGATE>
template <typename SamplesFWIterator, typename HandlesFWIterator>
void
DataWriter<T, DELEGATE>::write(const SamplesFWIterator& data_begin,
        const SamplesFWIterator& data_end,
        const HandlesFWIterator& handle_begin,
        const HandlesFWIterator& handle_end)
{
    SamplesFWIterator data = data_begin;
    HandlesFWIterator handle = handle_begin;

    while(data != data_end && handle != handle_end)
    {
        this->delegate()->write(*data, *handle);
        ++data;
        ++handle;
    }
}

template <typename T, template <typename Q> class DELEGATE>
template <typename SamplesFWIterator, typename HandlesFWIterator>
void
DataWriter<T, DELEGATE>::write(const SamplesFWIterator& data_begin,
        const SamplesFWIterator& data_end,
        const HandlesFWIterator& handle_begin,
        const HandlesFWIterator& handle_end,
        const dds::core::Time& timestamp)
{
    SamplesFWIterator data = data_begin;
    HandlesFWIterator handle = handle_begin;

    while(data != data_end && handle != handle_end)
    {
        this->delegate()->write(*data, *handle, timestamp);
        ++data;
        ++handle;
    }
}

template <typename T, template <typename Q> class DELEGATE>
DataWriter<T, DELEGATE>&
DataWriter<T, DELEGATE>::operator <<(const ::dds::pub::qos::DataWriterQos& qos)
{
    this->delegate()->qos(qos);
    return *this;
}

template <typename T, template <typename Q> class DELEGATE>
DataWriter<T, DELEGATE>&
DataWriter<T, DELEGATE>::operator << (const T& data)
{
    this->write(data);
    return *this;
}

template <typename T, template <typename Q> class DELEGATE>
DataWriter<T, DELEGATE>&
DataWriter<T, DELEGATE>::operator << (const std::pair<T, dds::core::Time>& data)
{
    this->write(data.first, data.second);
    return *this;
}

template <typename T, template <typename Q> class DELEGATE>
DataWriter<T, DELEGATE>&
DataWriter<T, DELEGATE>::operator << (const std::pair<T, ::dds::core::InstanceHandle>& data)
{
    this->write(data.first, data.second);
    return *this;
}

template <typename T, template <typename Q> class DELEGATE>
DataWriter<T, DELEGATE>&
DataWriter<T, DELEGATE>::operator <<(DataWriter & (*manipulator)(DataWriter&))
{
    return manipulator(*this);
}

template <typename T, template <typename Q> class DELEGATE>
const dds::core::InstanceHandle
DataWriter<T, DELEGATE>::register_instance(const T& key)
{
    /* Invalid time will be used as current time. */
    return this->delegate()->register_instance(key, dds::core::Time::invalid());
}

template <typename T, template <typename Q> class DELEGATE>
const dds::core::InstanceHandle
DataWriter<T, DELEGATE>::register_instance(const T& key,
        const dds::core::Time& timestamp)
{
    return this->delegate()->register_instance(key, timestamp);
}

template <typename T, template <typename Q> class DELEGATE>
DataWriter<T, DELEGATE>&
DataWriter<T, DELEGATE>::unregister_instance(const ::dds::core::InstanceHandle& i)
{
    /* Invalid time will be used as current time. */
    this->delegate()->unregister_instance(i, dds::core::Time::invalid());
    return *this;
}

template <typename T, template <typename Q> class DELEGATE>
DataWriter<T, DELEGATE>&
DataWriter<T, DELEGATE>::unregister_instance(const ::dds::core::InstanceHandle& i,
        const dds::core::Time& timestamp)
{
    this->delegate()->unregister_instance(i, timestamp);
    return *this;
}

template <typename T, template <typename Q> class DELEGATE>
DataWriter<T, DELEGATE>&
DataWriter<T, DELEGATE>::unregister_instance(const T& key)
{
    /* Invalid time will be used as current time. */
    this->delegate()->unregister_instance(key, dds::core::Time::invalid());
    return *this;
}

template <typename T, template <typename Q> class DELEGATE>
DataWriter<T, DELEGATE>&
DataWriter<T, DELEGATE>::unregister_instance(const T& key,
        const dds::core::Time& timestamp)
{
    this->delegate()->unregister_instance(key, timestamp);
    return *this;
}

template <typename T, template <typename Q> class DELEGATE>
DataWriter<T, DELEGATE>&
DataWriter<T, DELEGATE>::dispose_instance(const ::dds::core::InstanceHandle& i)
{
    /* Invalid time will be used as current time. */
    this->delegate()->dispose_instance(i, dds::core::Time::invalid());
    return *this;
}

template <typename T, template <typename Q> class DELEGATE>
DataWriter<T, DELEGATE>&
DataWriter<T, DELEGATE>::dispose_instance(const ::dds::core::InstanceHandle& i,
        const dds::core::Time& timestamp)
{
    this->delegate()->dispose_instance(i, timestamp);
    return *this;
}

template <typename T, template <typename Q> class DELEGATE>
DataWriter<T, DELEGATE>&
DataWriter<T, DELEGATE>::dispose_instance(const T& key)
{
    /* Invalid time will be used as current time. */
    this->delegate()->dispose_instance(key, dds::core::Time::invalid());
    return *this;
}

template <typename T, template <typename Q> class DELEGATE>
DataWriter<T, DELEGATE>&
DataWriter<T, DELEGATE>::dispose_instance(const T& key,
        const dds::core::Time& timestamp)
{
    this->delegate()->dispose_instance(key, timestamp);
    return *this;
}

template <typename T, template <typename Q> class DELEGATE>
dds::topic::TopicInstance<T>&
DataWriter<T, DELEGATE>::key_value(dds::topic::TopicInstance<T>& i,
        const ::dds::core::InstanceHandle& h)
{
    return this->delegate()->key_value(i, h);
}

template <typename T, template <typename Q> class DELEGATE>
T&
DataWriter<T, DELEGATE>::key_value(T& sample, const ::dds::core::InstanceHandle& h)
{
    return this->delegate()->key_value(sample, h);
}

template <typename T, template <typename Q> class DELEGATE>
dds::core::InstanceHandle
DataWriter<T, DELEGATE>::lookup_instance(const T& key)
{
    return this->delegate()->lookup_instance(key);
}

template <typename T, template <typename Q> class DELEGATE>
const dds::topic::Topic<T>&
DataWriter<T, DELEGATE>::topic() const
{
    return this->delegate()->topic();
}

template <typename T, template <typename Q> class DELEGATE>
void
DataWriter<T, DELEGATE>::listener(DataWriterListener<T>* listener,
    const ::dds::core::status::StatusMask& mask)
{
    this->delegate()->listener(listener, mask);
}

template <typename T, template <typename Q> class DELEGATE>
DataWriterListener<T>*
DataWriter<T, DELEGATE>::listener() const
{
    return this->delegate()->listener();
}

}
}




/***************************************************************************
 *
 * dds/pub/detail/DataWriter<> DELEGATE implementation.
 * Declaration can be found in dds/pub/detail/DataWriter.hpp
 *
 * Implementation and declaration have been separated because some circular
 * dependencies, like with DataWriterListener and AnyDataWriter.
 *
 ***************************************************************************/

#include <dds/pub/AnyDataWriter.hpp>
#include <dds/pub/DataWriterListener.hpp>
#include <org/eclipse/cyclonedds/pub/AnyDataWriterDelegate.hpp>

template <typename T>
dds::pub::detail::DataWriter<T>::DataWriter(
    const dds::pub::Publisher& pub,
    const ::dds::topic::Topic<T>& topic,
    const dds::pub::qos::DataWriterQos& qos)
    : ::org::eclipse::cyclonedds::pub::AnyDataWriterDelegate(qos, topic), pub_(pub), topic_(topic)
{
    DDSCXX_WARNING_MSVC_OFF(6326)
    if (dds::topic::is_topic_type<T>::value == 0) {
        ISOCPP_THROW_EXCEPTION(ISOCPP_PRECONDITION_NOT_MET_ERROR, "DataWriter cannot be created, topic information not found");
    }
    DDSCXX_WARNING_MSVC_ON(6326)

    org::eclipse::cyclonedds::pub::qos::DataWriterQosDelegate dwQos = qos.delegate();

    dds_entity_t ddsc_pub = static_cast<dds_entity_t>(pub.delegate()->get_ddsc_entity());
    dds_entity_t ddsc_topic = static_cast<dds_entity_t>(topic.delegate()->get_ddsc_entity());

    // get and validate the ddsc qos
    dwQos.check();
    dds_qos_t *ddsc_qos = dwQos.ddsc_qos();

    std::string name = topic.name() + "_datawriter";

    this->listener_set(nullptr, dds::core::status::StatusMask::all(), false);
    dds_entity_t ddsc_writer = dds_create_writer (ddsc_pub, ddsc_topic, ddsc_qos, this->listener_callbacks);
    dds_delete_qos(ddsc_qos);
    ISOCPP_DDSC_RESULT_CHECK_AND_THROW(ddsc_writer, "Could not create DataWriter.");
    topic_.delegate()->incrNrDependents();

    this->set_ddsc_entity(ddsc_writer);
}

template <typename T>
dds::pub::detail::DataWriter<T>::~DataWriter<T>()
{
    if (!this->closed) {
        try {
            this->close();
        } catch (...) {
            /* Empty: the exception throw should have already traced an error. */
        }
    }
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::init(ObjectDelegate::weak_ref_type weak_ref)
{
    /* Set weak_ref before passing ourselves to other isocpp objects. */
    this->set_weak_ref(weak_ref);
    /* Add weak_ref to the map of entities */
    this->add_to_entity_map(weak_ref);
    /* Register writer at publisher. */
    this->pub_.delegate()->add_datawriter(*this);

    /* Enable when needed. */
    if (this->pub_.delegate()->is_auto_enable()) {
        this->enable();
    }
}

template <typename T>
bool
dds::pub::detail::DataWriter<T>::is_loan_supported()
{
  this->check();
  return AnyDataWriterDelegate::is_loan_supported(static_cast<dds_entity_t>(this->ddsc_entity));
}

template <typename T>
T&
dds::pub::detail::DataWriter<T>::loan_sample()
{
    T *sample;
    this->check();
    AnyDataWriterDelegate::loan_sample(static_cast<dds_entity_t>(this->ddsc_entity), reinterpret_cast<void **>(&sample));
    return *sample;
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::return_loan(T& sample)
{
    this->check();
    AnyDataWriterDelegate::return_loan(static_cast<dds_entity_t>(this->ddsc_entity), &sample);
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::write_cdr(const org::eclipse::cyclonedds::topic::CDRBlob& sample)
{
    this->check();
    AnyDataWriterDelegate::write_cdr(static_cast<dds_entity_t>(this->ddsc_entity),
                                  &sample,
                                  dds::core::InstanceHandle(dds::core::null),
                                  dds::core::Time::invalid());
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::write_cdr(
            const org::eclipse::cyclonedds::topic::CDRBlob& sample,
            const dds::core::Time& timestamp)
{
    this->check();
    AnyDataWriterDelegate::write_cdr(static_cast<dds_entity_t>(this->ddsc_entity),
                                  &sample,
                                  dds::core::InstanceHandle(dds::core::null),
                                  timestamp);
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::dispose_cdr(const org::eclipse::cyclonedds::topic::CDRBlob& sample)
{
    this->check();
    AnyDataWriterDelegate::dispose_cdr(static_cast<dds_entity_t>(this->ddsc_entity),
                                  &sample,
                                  dds::core::InstanceHandle(dds::core::null),
                                  dds::core::Time::invalid());
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::dispose_cdr(
            const org::eclipse::cyclonedds::topic::CDRBlob& sample,
            const dds::core::Time& timestamp)
{
    this->check();
    AnyDataWriterDelegate::dispose_cdr(static_cast<dds_entity_t>(this->ddsc_entity),
                                  &sample,
                                  dds::core::InstanceHandle(dds::core::null),
                                  timestamp);
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::unregister_instance_cdr(const org::eclipse::cyclonedds::topic::CDRBlob& sample)
{
    this->check();
    AnyDataWriterDelegate::unregister_instance_cdr(static_cast<dds_entity_t>(this->ddsc_entity),
                                  &sample,
                                  dds::core::InstanceHandle(dds::core::null),
                                  dds::core::Time::invalid());
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::unregister_instance_cdr(
            const org::eclipse::cyclonedds::topic::CDRBlob& sample,
            const dds::core::Time& timestamp)
{
    this->check();
    AnyDataWriterDelegate::unregister_instance_cdr(static_cast<dds_entity_t>(this->ddsc_entity),
                                  &sample,
                                  dds::core::InstanceHandle(dds::core::null),
                                  timestamp);
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::write(const T& sample)
{
    this->check();
    AnyDataWriterDelegate::write(static_cast<dds_entity_t>(this->ddsc_entity),
                                  &sample,
                                  dds::core::InstanceHandle(dds::core::null),
                                  dds::core::Time::invalid());
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::write(const T& sample, const dds::core::Time& timestamp)
{
    this->check();
    AnyDataWriterDelegate::write(static_cast<dds_entity_t>(this->ddsc_entity),
                                  &sample,
                                  dds::core::InstanceHandle(dds::core::null),
                                  timestamp);
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::write(const T& sample, const ::dds::core::InstanceHandle& instance)
{
    this->check();
    AnyDataWriterDelegate::write(static_cast<dds_entity_t>(this->ddsc_entity),
                                  &sample,
                                  instance,
                                  dds::core::Time::invalid());
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::write(const T& sample,
           const ::dds::core::InstanceHandle& instance,
           const dds::core::Time& timestamp)
{
    this->check();
    AnyDataWriterDelegate::write(static_cast<dds_entity_t>(this->ddsc_entity),
                                  &sample,
                                  instance,
                                  timestamp);
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::write(const dds::topic::TopicInstance<T>& i)
{
    this->check();
    AnyDataWriterDelegate::write(static_cast<dds_entity_t>(this->ddsc_entity),
                                  &i.sample(),
                                  i.handle(),
                                  dds::core::Time::invalid());
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::write(const dds::topic::TopicInstance<T>& i,
           const dds::core::Time& timestamp)
{
    this->check();
    AnyDataWriterDelegate::write(static_cast<dds_entity_t>(this->ddsc_entity),
                                  &i.sample(),
                                  i.handle(),
                                  timestamp);
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::writedispose(const T& sample)
{
    this->check();
    AnyDataWriterDelegate::writedispose(
                                  static_cast<dds_entity_t>(this->ddsc_entity),
                                  &sample,
                                  dds::core::InstanceHandle(dds::core::null),
                                  dds::core::Time::invalid());
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::writedispose(const T& sample, const dds::core::Time& timestamp)
{
    this->check();
    AnyDataWriterDelegate::writedispose(
                                  static_cast<dds_entity_t>(this->ddsc_entity),
                                  &sample,
                                  dds::core::InstanceHandle(dds::core::null),
                                  timestamp);
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::writedispose(const T& sample, const ::dds::core::InstanceHandle& instance)
{
    this->check();
    AnyDataWriterDelegate::writedispose(
                                  static_cast<dds_entity_t>(this->ddsc_entity),
                                  &sample,
                                  instance,
                                  dds::core::Time::invalid());
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::writedispose(
           const T& sample,
           const ::dds::core::InstanceHandle& instance,
           const dds::core::Time& timestamp)
{
    this->check();
    AnyDataWriterDelegate::writedispose(
                                  static_cast<dds_entity_t>(this->ddsc_entity),
                                  &sample,
                                  instance,
                                  timestamp);
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::writedispose(const dds::topic::TopicInstance<T>& i)
{
    this->check();
    AnyDataWriterDelegate::writedispose(
                                  static_cast<dds_entity_t>(this->ddsc_entity),
                                  &i.sample(),
                                  i.handle(),
                                  dds::core::Time::invalid());
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::writedispose(
           const dds::topic::TopicInstance<T>& i,
           const dds::core::Time& timestamp)
{
    this->check();
    AnyDataWriterDelegate::writedispose(
                                  static_cast<dds_entity_t>(this->ddsc_entity),
                                  &i.sample(),
                                  i.handle(),
                                  timestamp);
}

template <typename T>
template <typename FWIterator>
void
dds::pub::detail::DataWriter<T>::writedispose(const FWIterator& begin, const FWIterator& end)
{
    FWIterator b = begin;
    while(b != end)
    {
        this->writedispose(*b);
        ++b;
    }
}

template <typename T>
template <typename FWIterator>
void
dds::pub::detail::DataWriter<T>::writedispose(const FWIterator& begin, const FWIterator& end,
        const dds::core::Time& timestamp)
{
    FWIterator b = begin;
    while(b != end)
    {
        this->writedispose(*b, timestamp);
        ++b;
    }
}

template <typename T>
template <typename SamplesFWIterator, typename HandlesFWIterator>
void
dds::pub::detail::DataWriter<T>::writedispose(
        const SamplesFWIterator& data_begin,
        const SamplesFWIterator& data_end,
        const HandlesFWIterator& handle_begin,
        const HandlesFWIterator& handle_end)
{
    SamplesFWIterator data = data_begin;
    HandlesFWIterator handle = handle_begin;

    while(data != data_end && handle != handle_end)
    {
        this->writedispose(*data, *handle);
        ++data;
        ++handle;
    }
}

template <typename T>
template <typename SamplesFWIterator, typename HandlesFWIterator>
void
dds::pub::detail::DataWriter<T>::writedispose(
        const SamplesFWIterator& data_begin,
        const SamplesFWIterator& data_end,
        const HandlesFWIterator& handle_begin,
        const HandlesFWIterator& handle_end,
        const dds::core::Time& timestamp)
{
    SamplesFWIterator data = data_begin;
    HandlesFWIterator handle = handle_begin;

    while(data != data_end && handle != handle_end)
    {
        this->writedispose(*data, *handle, timestamp);
        ++data;
        ++handle;
    }
}

template <typename T>
const ::dds::core::InstanceHandle
dds::pub::detail::DataWriter<T>::register_instance(const T& key,
                                                   const dds::core::Time& timestamp)
{
    org::eclipse::cyclonedds::core::ScopedObjectLock scopedLock(*this);
    this->check();
    dds::core::InstanceHandle handle(AnyDataWriterDelegate::register_instance(static_cast<dds_entity_t>(this->ddsc_entity), &key, timestamp));
    return handle;
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::unregister_instance(const ::dds::core::InstanceHandle& handle,
                                                     const dds::core::Time& timestamp)
{
    org::eclipse::cyclonedds::core::ScopedObjectLock scopedLock(*this);
    this->check();
    AnyDataWriterDelegate::unregister_instance(static_cast<dds_entity_t>(this->ddsc_entity), handle, timestamp);
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::unregister_instance(const T& sample,
                                                     const dds::core::Time& timestamp)
{
    org::eclipse::cyclonedds::core::ScopedObjectLock scopedLock(*this);
    this->check();
    AnyDataWriterDelegate::unregister_instance(static_cast<dds_entity_t>(this->ddsc_entity), &sample, timestamp);
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::dispose_instance(const ::dds::core::InstanceHandle& handle,
                                                  const dds::core::Time& timestamp)
{
    org::eclipse::cyclonedds::core::ScopedObjectLock scopedLock(*this);
    this->check();
    AnyDataWriterDelegate::dispose_instance(static_cast<dds_entity_t>(this->ddsc_entity), handle, timestamp);
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::dispose_instance(const T& sample,
                                                  const dds::core::Time& timestamp)
{
    org::eclipse::cyclonedds::core::ScopedObjectLock scopedLock(*this);
    this->check();
    AnyDataWriterDelegate::dispose_instance(static_cast<dds_entity_t>(this->ddsc_entity), &sample, timestamp);
}


template <typename T>
dds::topic::TopicInstance<T>&
dds::pub::detail::DataWriter<T>::key_value(dds::topic::TopicInstance<T>& i,
                                           const ::dds::core::InstanceHandle& h)
{
    org::eclipse::cyclonedds::core::ScopedObjectLock scopedLock(*this);
    this->check();
    T sample;
    AnyDataWriterDelegate::get_key_value(static_cast<dds_entity_t>(this->ddsc_entity), &sample, h);
    i.handle(h);
    i.sample(sample);
    return i;
}

template <typename T>
T&
dds::pub::detail::DataWriter<T>::key_value(T& sample, const ::dds::core::InstanceHandle& h)
{
    org::eclipse::cyclonedds::core::ScopedObjectLock scopedLock(*this);
    this->check();
    AnyDataWriterDelegate::get_key_value(static_cast<dds_entity_t>(this->ddsc_entity), &sample, h);
    return sample;
}

template <typename T>
dds::core::InstanceHandle
dds::pub::detail::DataWriter<T>::lookup_instance(const T& key)
{
    org::eclipse::cyclonedds::core::ScopedObjectLock scopedLock(*this);
    this->check();
    dds::core::InstanceHandle handle(AnyDataWriterDelegate::lookup_instance(static_cast<dds_entity_t>(this->ddsc_entity), &key));
    return handle;
}

template <typename T>
const dds::topic::Topic<T>&
dds::pub::detail::DataWriter<T>::topic() const
{
    org::eclipse::cyclonedds::core::ScopedObjectLock scopedLock(*this);
    this->check();
    return this->topic_;
}

template <typename T>
const dds::pub::Publisher&
dds::pub::detail::DataWriter<T>::publisher() const
{
    org::eclipse::cyclonedds::core::ScopedObjectLock scopedLock(*this);
    this->check();
    return this->pub_;
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::listener(DataWriterListener<T>* listener,
                                   const ::dds::core::status::StatusMask& mask)
{
    org::eclipse::cyclonedds::core::ScopedObjectLock scopedLock(*this);
    this->check();
    this->listener_set(listener, mask, true);
}

template <typename T>
dds::pub::DataWriterListener<T>*
dds::pub::detail::DataWriter<T>::listener() const
{
    org::eclipse::cyclonedds::core::ScopedObjectLock scopedLock(*this);
    this->check();
    return reinterpret_cast<dds::pub::DataWriterListener<T>*>(this->listener_get());
}

template <typename T>
void
dds::pub::detail::DataWriter<T>::close()
{
    org::eclipse::cyclonedds::core::ScopedObjectLock scopedLock(*this);

    this->listener_set(NULL, dds::core::status::StatusMask::none(), true);

    topic_.delegate()->decrNrDependents();

    this->pub_.delegate()->remove_datawriter(*this);

    org::eclipse::cyclonedds::pub::AnyDataWriterDelegate::close();

    scopedLock.unlock();
}

template <typename T>
dds::pub::DataWriter<T, dds::pub::detail::DataWriter>
dds::pub::detail::DataWriter<T>::wrapper()
{
    typename DataWriter::ref_type ref =
            ::std::dynamic_pointer_cast<DataWriter<T> >(this->get_strong_ref());
    dds::pub::DataWriter<T, dds::pub::detail::DataWriter> writer(ref);

    return writer;
}

template <typename T>
void dds::pub::detail::DataWriter<T>::on_offered_deadline_missed(dds_entity_t,
             org::eclipse::cyclonedds::core::OfferedDeadlineMissedStatusDelegate &sd)
{
    dds::core::status::OfferedDeadlineMissedStatus s;
    s.delegate() = sd;

    auto sr = this->get_strong_ref();
    if (sr) {
        dds::pub::DataWriter<T, dds::pub::detail::DataWriter> dw = wrapper();

        dds::pub::DataWriterListener<T> *l =
                reinterpret_cast<dds::pub::DataWriterListener<T> *>(this->listener_get());
        if (l)
          l->on_offered_deadline_missed(dw, s);
    }
}

template <typename T>
void dds::pub::detail::DataWriter<T>::on_offered_incompatible_qos(dds_entity_t,
             org::eclipse::cyclonedds::core::OfferedIncompatibleQosStatusDelegate &sd)
{
    dds::core::status::OfferedIncompatibleQosStatus s;
    s.delegate() = sd;

    auto sr = this->get_strong_ref();
    if (sr) {
        dds::pub::DataWriter<T, dds::pub::detail::DataWriter> dw = wrapper();

        dds::pub::DataWriterListener<T> *l =
                reinterpret_cast<dds::pub::DataWriterListener<T> *>(this->listener_get());
        if (l)
          l->on_offered_incompatible_qos(dw, s);
    }
}

template <typename T>
void dds::pub::detail::DataWriter<T>::on_liveliness_lost(dds_entity_t,
             org::eclipse::cyclonedds::core::LivelinessLostStatusDelegate &sd)
{
    dds::core::status::LivelinessLostStatus s;
    s.delegate() = sd;

    auto sr = this->get_strong_ref();
    if (sr) {
        dds::pub::DataWriter<T, dds::pub::detail::DataWriter> dw = wrapper();

        dds::pub::DataWriterListener<T> *l =
                reinterpret_cast<dds::pub::DataWriterListener<T> *>(this->listener_get());
        if (l)
          l->on_liveliness_lost(dw, s);
    }
}

template <typename T>
void dds::pub::detail::DataWriter<T>::on_publication_matched(dds_entity_t,
             org::eclipse::cyclonedds::core::PublicationMatchedStatusDelegate &sd)
{
    dds::core::status::PublicationMatchedStatus s;
    s.delegate() = sd;

    auto sr = this->get_strong_ref();
    if (sr) {
        dds::pub::DataWriter<T, dds::pub::detail::DataWriter> dw = wrapper();

        dds::pub::DataWriterListener<T> *l =
                reinterpret_cast<dds::pub::DataWriterListener<T> *>(this->listener_get());
        if (l)
          l->on_publication_matched(dw, s);
    }
}

#endif /* OMG_DDS_PUB_DATA_WRITER_IMPL_HPP_ */
