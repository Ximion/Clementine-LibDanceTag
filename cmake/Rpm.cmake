set(RPMBUILD_DIR ~/rpmbuild CACHE STRING "Rpmbuild directory, for the rpm target")
set(MOCK_CHROOT fedora-13-x86_64 CACHE STRING "Chroot to use when building an rpm with mock")
set(RPM_DISTRO fc13 CACHE STRING "Suffix of the rpm file")
set(RPM_ARCH x86_64 CACHE STRING "Architecture of the rpm file")

add_custom_target(rpm
  COMMAND ${CMAKE_SOURCE_DIR}/dist/maketarball.sh
  COMMAND ${CMAKE_COMMAND} -E copy clementine-${CLEMENTINE_VERSION_RPM}.tar.gz ${RPMBUILD_DIR}/SOURCES/
  COMMAND rpmbuild -bs ${CMAKE_SOURCE_DIR}/dist/clementine.spec
  COMMAND mock -v -r ${MOCK_CHROOT} ${RPMBUILD_DIR}/SRPMS/clementine-${CLEMENTINE_VERSION_RPM}-1.${RPM_DISTRO}.src.rpm
  COMMAND ${CMAKE_COMMAND} -E copy /var/lib/mock/${MOCK_CHROOT}/result/clementine-${CLEMENTINE_VERSION_RPM}-1.${RPM_DISTRO}.${RPM_ARCH}.rpm
                                   ${CMAKE_BINARY_DIR}/clementine-${CLEMENTINE_VERSION_RPM}-1.${RPM_DISTRO}.${RPM_ARCH}.rpm
)
