# syntax=docker/dockerfile:1

# Comments are provided throughout this file to help you get started.
# If you need more help, visit the Dockerfile reference guide at
# https://docs.docker.com/go/dockerfile-reference/

# Want to help us make this template better? Share your feedback here: https://forms.gle/ybq9Krt8jtBL3iCk7

ARG PYTHON_VERSION=3.10.9
FROM python:${PYTHON_VERSION}-slim as base

# Prevents Python from writing pyc files.
ENV PYTHONDONTWRITEBYTECODE=1

# Keeps Python from buffering stdout and stderr to avoid situations where
# the application crashes without emitting any logs due to buffering.
ENV PYTHONUNBUFFERED=1

WORKDIR /app

# Create a non-privileged user that the app will run under.
# See https://docs.docker.com/go/dockerfile-user-best-practices/
ARG UID=10001
RUN adduser \
    --disabled-password \
    --gecos "" \
    --home "/nonexistent" \
    --shell "/sbin/nologin" \
    --no-create-home \
    --uid "${UID}" \
    appuser

# Download dependencies as a separate step to take advantage of Docker's caching.
# Leverage a cache mount to /root/.cache/pip to speed up subsequent builds.
# Leverage a bind mount to requirements.txt to avoid having to copy them into
# into this layer.
RUN --mount=type=cache,target=/root/.cache/pip \
    --mount=type=bind,source=requirements.txt,target=requirements.txt \
    python -m pip install -r requirements.txt

# Install FFmpeg
RUN apt-get -y update && apt-get -y upgrade && apt-get install -y --no-install-recommends ffmpeg

# Install Bento4
ENV BENTO4_VERSION=1.6.0-637
ENV BENTO4_INSTALL_DIR=/opt/bento4
ENV PATH=/opt/bento4/bin:${PATH}

# Install dependencies.
RUN apt-get update && apt-get install -y --no-install-recommends \
    bash \
    python3 \
    wget \
    make \
    gcc \
    g++

# Fetch source.
RUN cd /tmp/ \
    && wget -O Bento4-${BENTO4_VERSION}.tar.gz https://github.com/axiomatic-systems/Bento4/archive/v${BENTO4_VERSION}.tar.gz \
    && tar -xzvf Bento4-${BENTO4_VERSION}.tar.gz && rm Bento4-${BENTO4_VERSION}.tar.gz

# Create installation directories.
RUN mkdir -p \
    ${BENTO4_INSTALL_DIR}/bin \
    ${BENTO4_INSTALL_DIR}/scripts \
    ${BENTO4_INSTALL_DIR}/include

# Build.
RUN cd /tmp/Bento4-${BENTO4_VERSION}/Build/Targets/x86-unknown-linux \
    && make AP4_BUILD_CONFIG=Release

# Install.
RUN cd /tmp \
    && cp -r Bento4-${BENTO4_VERSION}/Build/Targets/x86-unknown-linux/Release/. ${BENTO4_INSTALL_DIR}/bin \
    && cp -r Bento4-${BENTO4_VERSION}/Source/Python/utils/. ${BENTO4_INSTALL_DIR}/utils \
    && cp -r Bento4-${BENTO4_VERSION}/Source/Python/wrappers/. ${BENTO4_INSTALL_DIR}/bin \
    && cp -r Bento4-${BENTO4_VERSION}/Source/C++/**/*.h . ${BENTO4_INSTALL_DIR}/include

# Cleanup.
RUN rm -rf /var/cache/apt/* /tmp/*

# Switch to the non-privileged user to run the application.
USER appuser

# Copy the source code into the container.
COPY . .

# Copy the video file to the /app directory in the Docker container
COPY BigBuckBunny.mp4 /app/


# Expose the port that the application listens on.
EXPOSE 5000

# Run the application.
CMD python3 -m flask run --host=0.0.0.0
