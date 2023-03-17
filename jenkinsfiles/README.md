## CI/CD Systems

CI/CD describes a process in software development which is used to automatically integrate code snippets of multiple contributors to maximize efficiency and minimize rework. Therefore a source code version control, such as git or svn, is needed. Additionally, there are various tools that can be used to verify the code when changes occur. Among other things, these tools are able to check coding styles, build projects and run unit tests and integration tests.

<br/>

There are a lot of different CI/CD Systems and three of them will be compared below:
* [Jenkins](https://www.jenkins.io/)
    * Pros:
        * Open source project with big community and support
        * Free license and a lot of free plugins 
        * Build in java, runs on all major platforms
        * Independent of repo hosting platform
        * Pipeline config via Jenkinsfile (script)
    * Cons:
        * Server, where jenkins runs has to be administrated
        * UI out of date
        * Storing job configurations isn't easy 
* [Gitlab CI](https://docs.gitlab.com/ee/ci/)
    * Pros:
        * Modern UI and up-to-date environment
        * Easy setup and configuration
        * Free to use
        * Jobs can be executed in parallel
        * [Pipeline config via .gitlab-ci.yml file](https://docs.gitlab.com/ee/ci/yaml/index.html#script)
    * Cons:
        * Complicated setup for github or other repos
        * Hosted by gitlab
    * [Connect GitLab CI with GitHub repo](https://docs.gitlab.com/ee/ci/ci_cd_for_external_repos/github_integration.html)
        * [GitLab repo](https://github.com/elfo23/jenkins_demo_ece)
* [GitHub Actions](https://docs.github.com/en/actions/automating-builds-and-tests/about-continuous-integration)
    * Pros:
        * Ideal integration for github repos
        * Free for public repos (unit ~2,000 minutes/month)
        * Easy job configuration with build-in editor and templates
        * Not only CI/CD tools available
        * [Pipeline config via .yml file](https://docs.github.com/en/actions/using-workflows/workflow-syntax-for-github-actions)
    * Cons:
        * Limited to github repos
        * Hosted by github

## Setup
You can find a detailed description on how to install and setup Jenkins in [this](../doc/CI/demo/) repository.

## Build, deploy and test DUT with jenkinsfile
Three different usecases were setup up using different jenkinsfiles. All three files were connected to separate jenkins-jobs and can be triggered manually or periodically. For the DUT the code [release/UNO_DUT_Car](../release/UNO_DUT_Car) was used. For the HW-Testing the code [release/ESP32_HW_Testing](../release/ESP32_HW_Testing/) was used.

* Usecase 1: [Build DUT](Jenkinsfile_DUT)
    * change to the DUT project directory
    * build the project with the help of the pio-cli
* Usecase 2: [Build, Flash, Test DUT](Jenkinsfile_DUT_Tests)
    * change to the DUT project directory
    * build the project with the help of the pio-cli
    * flash onto Arduino Uno
    * run unit-tests on Uno
* Usecase 3: [Build, Flash Hw-Tests](Jenkinsfile_DUT_HW_Tests)
    * change to the DUT project directory
    * build and flash onto Uno
    * change to the HW-Mock project directory
    * build and flash onto ESP32
    * run test onto ESP32 to test Uno

### Code snippets
With the help of the the **dir-step**, the current working directory can be changed to the required project folder.
```groovy
dir("release/UNO_DUT_Car/") 
{
    echo 'Build stage: compile release folder of DUT'
}
```

Inside the **post-step** it's possible to define specific events for three job states (success, failure, always). For example if the jenkins run was successful the post event under the tag "success" will be triggered.
```groovy
post
{
    failure
    {
        script
        {
            echo 'an error occurred'
        }
    }
    success
    {
        script
        {
            echo 'run was successful'
        }
    }
}
```

With the help of the **sh-statement**, it's possible to run shell script commands.
``` groovy
sh 'pio test'
```

## Useful links
* Pipline syntax: https://www.jenkins.io/doc/book/pipeline/syntax/
* Simple example: https://www.jenkins.io/doc/pipeline/tour/hello-world/