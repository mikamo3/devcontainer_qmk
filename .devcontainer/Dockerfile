FROM qmkfm/base_container

ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID
ARG TAG=

RUN python3 -m pip install -U pip wheel
RUN python3 -m pip install -U qmk
RUN apt-get update && apt-get install -y sudo && rm -rf /var/lib/apt/lists/*


RUN useradd -u ${USER_UID} -G sudo -m -s /bin/bash ${USERNAME} && \
  echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

RUN mkdir -p /qmk_firmware && chown ${USERNAME}.${USERNAME} /qmk_firmware

USER ${USERNAME}
WORKDIR /qmk_firmware
RUN git clone --recursive https://github.com/qmk/qmk_firmware.git /qmk_firmware
RUN if [ "${TAG}" != "" ]; then echo "${TAG}"; git checkout -b "${TAG}" "${TAG}"; fi
RUN qmk setup -y