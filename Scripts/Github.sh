if (( $# != 1 )); then
	echo "Illegal number of parameters. Usage Github.sh message"
	exit
fi

pushd ../Tools/Scripts
./createfullproject_noeditor.sh EasingDemo --rebuild --direct3d vs2013
./createGithubProject.sh EasingDemo vs2013
popd

NOW=$(date +"%c")
Message="$1 : Synced on $NOW "
pushd ../../EasingDemo_Github 
set BRANCH = "remote"
git add -A :/
git commit -a -m "$Message"
git pull $BRANCH
git push $BRANCH
popd
