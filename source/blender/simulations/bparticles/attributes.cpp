#include "attributes.hpp"

namespace BParticles {

AttributesInfo::AttributesInfo(ArrayRef<std::string> byte_names,
                               ArrayRef<std::string> float_names,
                               ArrayRef<std::string> float3_names)
{
  m_indices = {};
  m_indices.add_multiple(byte_names);
  m_indices.add_multiple(float_names);
  m_indices.add_multiple(float3_names);
  BLI_assert(m_indices.size() == byte_names.size() + float_names.size() + float3_names.size());

  m_byte_attributes = Range<uint>(0, byte_names.size());
  m_float_attributes = m_byte_attributes.after(float_names.size());
  m_float3_attributes = m_float_attributes.after(float3_names.size());

  m_types = {};
  m_types.append_n_times(AttributeType::Byte, m_byte_attributes.size());
  m_types.append_n_times(AttributeType::Float, m_float_attributes.size());
  m_types.append_n_times(AttributeType::Float3, m_float3_attributes.size());

  m_byte_defaults.append_n_times(0, m_byte_attributes.size());
  m_float_defaults.append_n_times(0, m_float_attributes.size());
  m_float3_defaults.append_n_times({0, 0, 0}, m_float3_attributes.size());
}

AttributeArraysCore::AttributeArraysCore(AttributesInfo &info, ArrayRef<void *> arrays, uint size)
    : m_info(&info), m_arrays(arrays.to_small_vector()), m_size(size)
{
}

AttributeArraysCore::~AttributeArraysCore()
{
}

AttributeArraysCore AttributeArraysCore::NewWithSeparateAllocations(AttributesInfo &info,
                                                                    uint size)
{
  SmallVector<void *> arrays;
  for (AttributeType type : info.types()) {
    arrays.append(MEM_malloc_arrayN(size, size_of_attribute_type(type), __func__));
  }
  return AttributeArraysCore(info, arrays, size);
}

void AttributeArraysCore::free_buffers()
{
  for (void *ptr : m_arrays) {
    MEM_freeN(ptr);
  }
}

};  // namespace BParticles
